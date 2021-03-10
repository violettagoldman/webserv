#!/usr/bin/env python3
from utils import TestBlock, TestCase, AbstractTester
import requests
import argparse


class BasicGet(TestCase):

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
    """
    Specify the path to the config file with which Webserv will be run
    """
    config_path = "../config/test_configs/integration_1.conf"
    nginx_config_path = "../config/test_configs/integration_1_nginx.conf"
    test_cases = [
        BasicGet()
    ]


class Tester(AbstractTester):
    """
    Specify all the test blocks to be run
    """
    blocks = [
        MainTestBlock()
    ]


def main():

    parser = argparse.ArgumentParser(description='Run tests on Webserver.')
    parser.add_argument('-p', type=int, default=8880, help="Specify a port to call")
    parser.add_argument('--nginx', default=True, action='store_false', help="Don't launch Webserv (useful when comparing to nginx)")
    args = parser.parse_args()

    # print(args.accumulate(args.integers))

    itests = Tester(args.p, args.nginx)
    itests.run()


if __name__ == '__main__':
    main()
