/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtoudic <mtoudic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 11:42:05 by mtoudic           #+#    #+#             */
/*   Updated: 2026/10/24 16:50:18 by mtoudic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdarg.h>
#include "ft_printf.h"

static int	putnbr(int n, int fd, const char *base)
{
	char	nstr[11];
	size_t	i;
	size_t	base_len;
	int		sign;

	sign = 1;
	if (n < 0)
		sign = -1;
	i = 10;
	base_len = ft_strlen(base);
	while (1)
	{
		nstr[i--] = base[(n % (int)base_len) * sign];
		n /= (int)base_len;
		if (n == 0)
			break ;
	}
	if (sign == -1)
		nstr[i] = '-';
	else
		i++;
	write(fd, nstr + i, 11 - i);
	return (11 - i);
}

static int	putunbr(unsigned int n, int fd, const char *base)
{
	char	nstr[11];
	size_t	i;
	size_t	base_len;

	i = 10;
	base_len = ft_strlen(base);
	while (1)
	{
		nstr[i--] = base[n % base_len];
		n /= base_len;
		if (n == 0)
			break ;
	}
	i++;
	write(fd, nstr + i, 11 - i);
	return (11 - i);
}

static int	putptr(unsigned int ptr, int fd, const char *base)
{
	char			nstr[13];
	size_t			i;
	size_t			base_len;
	unsigned int	n;

	i = 12;
	n = ((size_t)ptr);
	base_len = ft_strlen(base);
	while (1)
	{
		nstr[i--] = base[n % base_len];
		n /= base_len;
		if (n == 0)
			break ;
	}
	nstr[i--] = 'x';
	nstr[i] = '0';
	write(fd, nstr + i, 13 - i);
	return (13 - i);
}

static int	cast_token(char c, va_list *args, int *writen)
{
	const char	*str;

	if (c == 'c' && ++(*writen))
		ft_putchar_fd((char)va_arg(*args, int), STDOUT_FILENO);
	else if (c == 's')
	{
		str = va_arg(*args, const char *);
		ft_putstr_fd((char *)str, STDOUT_FILENO);
		*writen += ft_strlen(str);
	}
	else if (c == 'p')
		*writen += putptr(va_arg(*args, unsigned int), 1, "0123456789abcdef");
	else if (c == 'd' || c == 'i')
		*writen += putnbr(va_arg(*args, int), 1, "0123456789");
	else if (c == 'u')
		*writen += putunbr(va_arg(*args, unsigned int), 1, "0123456789");
	else if (c == 'x')
		*writen += putunbr(va_arg(*args, unsigned int), 1, "0123456789abcdef");
	else if (c == 'X')
		*writen += putunbr(va_arg(*args, unsigned int), 1, "0123456789ABCDEF");
	else if (c == '%' && ++(*writen))
		ft_putchar_fd('%', STDOUT_FILENO);
	else
		return (0);
	return (1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	strend;
	int		writen;

	va_start(args, format);
	strend = 0;
	writen = 0;
	while (format[strend] != '\0')
	{
		while (format[strend] != '\0' && format[strend] != '%')
			strend++;
		write(STDOUT_FILENO, format, strend);
		writen += strend;
		if (format[strend] == '%')
			strend += cast_token(format[strend + 1], &args, &writen) + 1;
		format += strend;
		strend = 0;
	}
	va_end(args);
	return (writen);
}
