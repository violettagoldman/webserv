#!/bin/bash
clang++ ../src/cgi/*.cpp ../src/Header.class.cpp ../src/Request.class.cpp ../src/get_next_line/*.cpp ./cgi/cgi_test_main.cpp ../src/Utility.cpp -fsanitize=address -I ../inc/ -o cgi_test
./cgi_test ; rm ./cgi_test
