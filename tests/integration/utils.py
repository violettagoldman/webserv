#!/usr/bin/env python3

from email.utils import parsedate_to_datetime
from datetime import datetime, timezone, timedelta
import subprocess
from time import sleep
import os
import shlex
import shutil


class Printer:
    """
    This class contains everything that you will need in all of other classes,
    like methods for color printing.
    """
    @staticmethod
    def print_fail(message):
        print('\x1b[1;31m' + message.strip() + '\x1b[0m', flush=True)

    @staticmethod
    def print_pass(message):
        print('\x1b[1;32m' + message.strip() + '\x1b[0m', flush=True)

    @staticmethod
    def print_info(message):
        print('\x1b[1;34m' + message.strip() + '\x1b[0m', flush=True)


# Abstract classes defining common behavior
class TestBlock(Printer):
    """
    A block of tests that will be run with a common config.
    """
    # The path to the config file which will be used to run Webserv
    config_path = ''

    # If the config is not compatible with Nginx (we use a couple of custom 
    # directives), set it in this attribute. By default, config_path will be used.
    nginx_config_path = ''

    # Add your TestCases to this list.
    test_cases = []

    def setup(self):
        """
        This method should be used to prepare for tests to be run.
        For example, copy the test files to a directory.
        """
        # Remove old tmp directory
        shutil.rmtree('/tmp/work')

        # Copy the test files to the tmp directory
        shutil.copytree('./test_files', '/tmp/work')

    def run_webserver(self):
        """
        Launch Webserv and save the process to kill later.
        """
        self.print_info("--- Webserver started ---")
        self.process = subprocess.Popen(['../../webserv', self.config_path])

    def run_nginx(self):
        """
        Launch a Nginx Docker container with the same (or corresponding) config.
        """
        used_config = self.config_path if self.nginx_config_path == '' else self.nginx_config_path

        true_config_path = os.path.join(os.getcwd(), used_config)
        s = f'docker run -it -p 80:80 -v /tmp/work:/tmp/work/ -v {true_config_path}:/etc/nginx/nginx.conf nginx'
        self.process = subprocess.Popen(shlex.split(s))

    def run(self, port=8880, launch_webserver=True):
        """
        The method that will be triggerred by the Tester. Launch all things
        from here.
        """
        self.print_info(f"--- Running test block {self.__class__.__name__} ---")
        if launch_webserver:
            self.run_webserver()
        else:
            port = 80
            self.run_nginx()

        # Let Nginx/webserv start
        sleep(3)

        # Run all tests
        for case in self.test_cases:
            case.run(port)

        # Kill the processes
        if launch_webserver:
            self.print_info("--- Killing webserver ---")
            self.process.kill()
        else:
            self.print_info("--- Killing nginx ---")
            self.process.terminate()


class TestCase(Printer):
    """
    One or many requests that test a single aspect of webserv's behavior.
    """

    description = ''
    webserv_url = 'http://127.0.0.1'

    def run(self, port):
        """
        This methods will be triggered by the TestBlock. Launch everything from
        here.
        """
        self.print_info(f"--- Running test case {self.__class__.__name__} ---")
        self.run_checks(port)

    def run_checks(self, port):
        """
        This method will be overwritten in subclasses to do the testing.
        """
        pass

    def check(self, description, value):
        """
        Use this method to check something. If value evaluates to True,
        prints [Pass], else prints [Fail] and description (for clarity).
        """
        if value:
            self.print_pass("[Pass]")
        else:
            self.print_fail(f"[Fail] -- {description}")

    def check_status_code(self, response, desired_code):
        """
        Checks that response's status code equals to desired_code.
        """
        if response.status_code == desired_code:
            self.print_pass("[Pass]")
        else:
            self.print_fail(f"[Fail] -- Expected status code: {desired_code}, got: {response.status_code}")

    def check_date_header(self, response):
        """
        Checks that response's Date header contains datetime that is +-
        10 seconds from now.
        """
        self.check('Date header present', 'date' in response.headers)
        response_datetime = parsedate_to_datetime(response.headers['date'])
        utc_now = datetime.now(timezone.utc)
        distance = utc_now - response_datetime
        self.check('Date in +- 10 seconds from now', distance < timedelta(seconds=10))


class AbstractTester:
    """
    All tests in one class. Pass port and launch_webserver to this class from
    main and call run().
    """

    blocks = None

    def __init__(self, port=8880, launch_webserver=True):
        self.port = port
        self.launch_webserver = launch_webserver

    def run(self):
        for block in self.blocks:
            block.run(self.port, self.launch_webserver)
