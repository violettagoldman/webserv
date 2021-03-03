#!/bin/bash
clang++ ../src/config/*.cpp ./config/config_test_main.cpp ../src/get_next_line/*.cpp ../src/Utility.cpp -I ../inc/ -o config_test -Wall -Wextra -Werror && ./config_test
rm config_test
