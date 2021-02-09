#!/bin/bash
clang++ ../src/config/*.cpp ./config/unit_tests.cpp ../src/get_next_line/*.cpp -I ../inc/ -o config_test && ./config_test ; rm config_test
