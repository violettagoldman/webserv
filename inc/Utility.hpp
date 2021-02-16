#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <algorithm>
# include <string>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <vector>

std::string		ft_itoa(int n);
std::string		readFile(std::string file);
std::string		replacehtml(std::string src, std::string var, std::string value);
std::vector<std::string> ft_split(std::string, char delimiter);

#endif