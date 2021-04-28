#!/bin/bash
clang++ ../src/config/*.cpp ../src/Base64.class.cpp ../src/cgi/TestCGIRequest.class.cpp ../src/Request.class.cpp ../src/Header.class.cpp ../src/cgi/CGIHandler.class.cpp ./config/config_test_main.cpp ../src/get_next_line/*.cpp ../src/Utility.cpp -I ../inc/ -o config_test -Wall -Wextra -Werror -g3 -fsanitize=address && ./config_test
rm config_test
