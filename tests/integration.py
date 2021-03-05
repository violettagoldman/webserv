import subprocess
import requests


# Abstract classes defining common behavior
class TestBlock:
    """
    A block of tests that will be run with a common config
    """
    config_path = 'lalal'
    test_cases = []

    def run_webserver(self):
        subprocess.run('./webserv', self.config_path)

    def run_all_cases(self):
        for case in self.test_cases:
            case.run()


class TestCase:
    """
    One or many requests that test a single aspect of webserv's behavior
    """
    def run():
        pass


# Example case and block
class WebserverRunsAtAll(TestCase):
    def run():
        requests.get('127.0.0.1')
        ...


class MainTestBlock(TestBlock):
    config_path = "whatever"
    test_cases = [
        WebserverRunsAtAll()
    ]


class IntegrationTests:
    """
    All tests in one class
    """

    blocks = [
        MainTestBlock()
    ]

    def run(self):
        for block in self.blocks:
            block.run_webserver()
            block.run_all_cases()


def main():
    itests = IntegrationTests()
    itests.run()


if __name__ == '__main__':
    main()
