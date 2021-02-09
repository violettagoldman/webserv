#!/bin/bash
clang++ ../src/cgi/*.cpp ../src/Header.class.cpp ../src/get_next_line/*.cpp ./cgi/test_main.cpp ../src/Utility.cpp -I ../inc/ -o cgi_test
./cgi_test ; rm ./cgi_test
