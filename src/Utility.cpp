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

time_t			getTime()
{
	struct timeval	tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);
	return (tv.tv_sec + tz.tz_minuteswest * 60);
}