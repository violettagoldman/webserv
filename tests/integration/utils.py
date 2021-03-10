#!/usr/bin/env python3

from email.utils import parsedate_to_datetime
from datetime import datetime, timezone, timedelta
import subprocess
from time import sleep
import os
import shlex


class Printer:
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
    A block of tests that will be run with a common config
    """
    config_path = ''
    test_cases = []

    def run_webserver(self):
        self.print_info("--- Webserver started ---")
        self.process = subprocess.Popen(['../../webserv', self.config_path])

    def run_nginx(self):
        true_config_path = os.path.join(os.getcwd(), self.nginx_config_path)

        print(true_config_path)

        s = f'docker run -it -p 80:80 -v /tmp/work:/tmp/work/ -v {true_config_path}:/etc/nginx/nginx.conf nginx'
        self.process = subprocess.Popen(shlex.split(s))

    def run(self, port=8880, launch_webserver=True):
        self.print_info(f"--- Running test block {self.__class__.__name__} ---")
        if launch_webserver:
            self.run_webserver()
            sleep(3)
        else:
            port = 80
            self.run_nginx()
            sleep(3)

        for case in self.test_cases:
            case.run(port)

        if launch_webserver:
            self.print_info("--- Killing webserver ---")
            self.process.kill()
        else:
            self.print_info("--- Killing nginx ---")
            self.process.terminate()


class TestCase(Printer):
    """
    One or many requests that test a single aspect of webserv's behavior
    """

    launch_webserver = True
    port = 8880
    description = ''

    def __init__(self, launch_webserver=True, port=8880):
        self.launch_webserver = launch_webserver
        self.port = port

    webserv_url = 'http://127.0.0.1'

    def run(self, port):
        self.print_info(f"--- Running test case {self.__class__.__name__} ---")
        self.run_checks(port)

    def run_checks(self, port):
        pass

    def check(self, description, value):
        if value:
            self.print_pass("[Pass]")
        else:
            self.print_fail(f"[Fail] -- {description}")

    def check_status_code(self, request, desired_code):
        if request.status_code == desired_code:
            self.print_pass("[Pass]")
        else:
            self.print_fail(f"[Fail] -- Expected status code: {desired_code}, got: {request.status_code}")

    def check_date_header(self, request):
        self.check('Date header present', 'date' in request.headers)
        request_datetime = parsedate_to_datetime(request.headers['date'])
        utc_now = datetime.now(timezone.utc)
        distance = utc_now - request_datetime
        self.check('Date in +- 10 seconds from now', distance < timedelta(seconds=10))


class AbstractTester:
    """
    All tests in one class
    """

    block = None

    def __init__(self, port=8880, launch_webserver=True):
        self.port = port
        self.launch_webserver = launch_webserver

    def run(self):
        for block in self.blocks:
            block.run(self.port, self.launch_webserver)
