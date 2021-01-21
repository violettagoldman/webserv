#include <algorithm>
#include <string>

std::string ft_itoa(int n)
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
