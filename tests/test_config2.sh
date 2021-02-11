#!/bin/bash
clang++ ../src/config/*.cpp new_config_main.cpp ../src/get_next_line/*.cpp -I ../inc/ -o config_test2 && ./config_test2 ; rm config_test2
