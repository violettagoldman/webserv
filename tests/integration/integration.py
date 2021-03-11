#!/usr/bin/env python3
from utils import TestBlock, TestCase, AbstractTester
import requests
import argparse


class BasicGet(TestCase):
    """
    Webserv must serve the file at /tmp/work/index.html to the GET request.
    """

    description = "Server serves index file at root"

    def run_checks(self, port):
        url = self.webserv_url + ':' + str(port) + '/'
        result = requests.get(url)
        self.check_status_code(result, 200)
        self.check('Correct content-type', result.headers['Content-Type'] == 'text/html')
        self.check_date_header(result)
        self.check('Response text', result.text == 'haha\n')
        # TODO: check last-modifieds


class MainTestBlock(TestBlock):

    # The config file for Webserv.
    config_path = "../config/test_configs/integration_1.conf"

    # The config file for Nginx mode, if different.
    nginx_config_path = "../config/test_configs/integration_1_nginx.conf"

    # TestCase instances that will be run against our Webserv/Nginx
    test_cases = [
        BasicGet()
    ]


class Tester(AbstractTester):
    """
    All TestBlocks that will be run from main
    """

    blocks = [
        MainTestBlock()
    ]


def main():

    # Console usage
    parser = argparse.ArgumentParser(description='Run tests on Webserver.')
    parser.add_argument('-p', type=int, default=8880, help="Specify a port to call")
    parser.add_argument('--nginx', default=True, action='store_false', help="Don't launch Webserv (useful when comparing to nginx)")
    args = parser.parse_args()

    # Create Tester and run all tests.
    itests = Tester(args.p, args.nginx)
    itests.run()


if __name__ == '__main__':
    main()
