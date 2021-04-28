#!/bin/bash

export REDIRECT_STATUS=200
export SERVER_PROTOCOL="HTTP/1.1"
export GATEWAY_INTERFACE="CGI/1.1"
export SCRIPT_FILENAME="/Users/ashishae/42/webserv/tests/cgi/cgi_tester"
export SCRIPT_NAME="/Users/ashishae/42/webserv/tests/cgi/cgi_tester"
export PATH_INFO="whatever"
export REQUEST_METHOD="GET"

/Users/ashishae/42/webserv/tests/cgi/cgi_tester
