#!/bin/bash

export REDIRECT_STATUS=200
export GATEWAY_INTERFACE="CGI/1.1"
export SCRIPT_FILENAME=/Users/ae/spacial-steel/cdng/webserv/cgi/tiny.php
export REQUEST_METHOD="GET"

/usr/local/bin/php-cgi
