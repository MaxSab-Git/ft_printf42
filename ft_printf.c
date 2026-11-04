/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtoudic <mtoudic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 11:42:05 by mtoudic           #+#    #+#             */
/*   Updated: 2026/11/04 11:00:07 by mtoudic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdarg.h>
#include <unistd.h>
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

static int	putunbr(size_t n, int fd, const char *base, int isptr)
{
	char			nstr[18];
	size_t			i;
	size_t			base_len;

	if (isptr && n == 0)
	{
		write(fd, "(nil)", 5);
		return (5);
	}
	i = 17;
	base_len = ft_strlen(base);
	while (1)
	{
		nstr[i--] = base[n % base_len];
		n /= base_len;
		if (n == 0)
			break ;
	}
	if (++i && isptr)
	{
		nstr[--i] = 'x';
		nstr[--i] = '0';
	}
	write(fd, nstr + i, 18 - i);
	return (18 - i);
}

static int	putstr(int fd, const char *str)
{
	size_t	len;

	if (str == NULL)
	{
		write(fd, "(null)", 6);
		return (6);
	}
	len = ft_strlen(str);
	write(fd, str, len);
	return (len);
}

static int	cast_token(char c, va_list *args, int *wr)
{
	int	wrs;

	wrs = 0;
	if (c == 'c' && ++(wrs))
		ft_putchar_fd((char)va_arg(*args, int), STDOUT_FILENO);
	else if (c == 's')
		wrs = putstr(STDOUT_FILENO, va_arg(*args, const char *));
	else if (c == 'p')
		wrs = putunbr((size_t)va_arg(*args, void *), 1, "0123456789abcdef", 1);
	else if (c == 'd' || c == 'i')
		wrs = putnbr(va_arg(*args, int), 1, "0123456789");
	else if (c == 'u')
		wrs = putunbr(va_arg(*args, unsigned int), 1, "0123456789", 0);
	else if (c == 'x')
		wrs = putunbr(va_arg(*args, unsigned int), 1, "0123456789abcdef", 0);
	else if (c == 'X')
		wrs = putunbr(va_arg(*args, unsigned int), 1, "0123456789ABCDEF", 0);
	else if (c == '%' && ++(wrs))
		ft_putchar_fd('%', STDOUT_FILENO);
	else
		return (0);
	*wr += wrs;
	if (wrs < 0)
		*wr = -1;
	return (1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	strend;
	int		writen;
	int		writes;

	if (format == NULL)
		return (-1);
	va_start(args, format);
	strend = 0;
	writen = 0;
	while (writen >= 0 && format[strend] != '\0')
	{
		while (format[strend] != '\0' && format[strend] != '%')
			strend++;
		writes = write(STDOUT_FILENO, format, strend);
		writen += writes;
		if (writes < 0)
			writen = -1;
		else if (format[strend] == '%')
			strend += cast_token(format[strend + 1], &args, &writen) + 1;
		format += strend;
		strend = 0;
	}
	va_end(args);
	return (writen);
}
