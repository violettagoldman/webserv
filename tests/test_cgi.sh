#!/bin/bash
clang++ ../src/cgi/*.cpp ../src/Header.class.cpp ../src/Request.class.cpp ../src/config/Exception.class.cpp ../src/get_next_line/*.cpp ./cgi/cgi_test_main.cpp ../src/Utility.cpp ../src/Base64.class.cpp -fsanitize=address -g3 -I ../inc/ -o cgi_test
./cgi_test ; rm ./cgi_test
