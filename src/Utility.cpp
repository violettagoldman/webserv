#include "Utility.hpp"

std::string 	ft_itoa(int n)
{
	std::string	res;
	int			nb;
	int			minus;

	res = "";
	nb = n;
	minus = 0;
	if (nb == -2147483648)
		return ("-2147483648");
	if (nb == 0)
		return ("0");
	if (nb < 0)
	{
		nb *= -1;
		minus = 1;
	}
	while (nb > 0)
	{
		res += ('0' + (nb % 10));
		nb /= 10;
	}
	std::reverse(res.begin(), res.end());
	if (minus)
		res = "-" + res;
	return (res);
}

std::string		readFile(std::string file)
{
	std::string	html;
	char		buffer[257] = {0};
	int			fd;

	html = "";
	if (!(fd = open(file.c_str(), O_CREAT)))
		return ""; // throw exception
	while (read(fd, buffer, 256) > 0)
	{
		html += std::string(buffer);
		for (size_t i = 0; i < 257; i++)
			buffer[i] = 0;
	}
	return (html);
}

std::string		replacehtml(std::string src, std::string var, std::string value)
{
	size_t	n;

	n = src.find(var);
	while (n != std::string::npos)
	{
		src.replace(n, var.size(), value);
		n = src.find(var, n + value.size());
	}
	return (src);
}

/*
** Split a string s, separated by delimiter c
** @param s String that needs to be splitted
** @param c The delimiter
** @ret std::vector<std::string> the vector of resulting separated strings
*/
std::vector<std::string> ft_split(std::string s, char c)
{
	std::vector<std::string> ret;
	if (s.size() == 0)
		return ret;
	int i = 0;
	size_t pos;
	while ((pos = s.find(c, i)) != std::string::npos)
	{
		ret.push_back(s.substr(i, pos-i));
		i = pos+1;
	}
	ret.push_back(s.substr(i));
	return ret;
}

time_t			getTime()
{
	struct timeval	tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);
	return (tv.tv_sec + tz.tz_minuteswest * 60);
}

// TODO : test some more
int	ft_atoi(const char *str)
{
	int		nbr;
	int		sign;

	nbr = 0;
	sign = 1;
	while ((*str) == '\t' || (*str) == '\n' || (*str) == '\v' || (*str) == '\f'
			|| (*str) == '\r' || (*str) == ' ')
		str++;
	if ((*str) == '-' || (*str) == '+')
	{
		sign *= ((*str) == '-' ? -1 : 1);
		str++;
	}
	while ((*str) != '\0' && (*str) >= '0' && (*str) <= '9')
	{
		nbr *= 10;
		nbr += (*str) - '0';
		str++;
	}
	return (nbr * sign);
}

size_t	ft_strlen(const char *str)
{
	char *start;

	start = (char *)str;
	while (*str)
		str++;
	return ((size_t)(str - start));
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	unsigned int i;
	unsigned int limit;

	i = 0;
	limit = (unsigned int)(dstsize - 1);
	if (!dst || !src)
		return (0);
	if (dstsize == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < limit)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

char *cppalloc(size_t size)
{
	char *ret = new char[size];
	return ret;
}

char	*ft_strdup(const char *s1)
{
	char	*p;
	size_t	len;

	len = ft_strlen(s1);
	if ((p = cppalloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	ft_strlcpy(p, s1, len + 1);
	p[len] = '\0';
	return (p);
}

std::string		ft_itostr(int n)
{
	std::string		result;
	long long int	nbr;

	if (n < 0)
		nbr = n == -2147483648 ? 2147483648 : -n;
	else
		nbr = n;
	do
	{
		result.insert(0, std::string(1, nbr % 10 + '0'));
		nbr /= 10;
	}
	while (nbr != 0);
	if (n < 0)
		result.insert(0, "-");
	return (result);
}
