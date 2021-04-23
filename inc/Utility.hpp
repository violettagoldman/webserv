#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <algorithm>
# include <string>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <sys/time.h>
# include <vector>

std::string					ft_itoa(int n);
std::string					readFile(std::string file);
std::string					replacehtml(std::string src, std::string var, std::string value);
time_t						getTime();
std::vector<std::string>	ft_split(std::string, char delimiter);
int							ft_atoi(const char *str);
size_t						ft_strlen(const char *str);
size_t						ft_strlcpy(char *dst, const char *src, size_t dstsize);
char						*cppalloc(size_t size);
char						*ft_strdup(const char *s1);
std::string					ft_itostr(int n);
void						ft_bzero(void *s, size_t n);

#endif