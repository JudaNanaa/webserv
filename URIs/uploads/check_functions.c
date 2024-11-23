/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:01:40 by madamou           #+#    #+#             */
/*   Updated: 2024/11/23 21:14:34 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_check_basic(const char *str, int i)
{
	if (str[i] == '%' && (str[i + 1] == 'c' || str[i + 1] == 's'
			|| str[i + 1] == 'd' || str[i + 1] == 'i'
			|| str[i + 1] == 'u' || str[i + 1] == 'x'
			|| str[i + 1] == 'X' || str[i + 1] == '%'
			|| str[i + 1] == 'p'))
		return (1);
	return (0);
}

char	*ft_check_if_format(const char *str, int i, char *print, va_list args)
{
	if (str[i] == '%' && str[i + 1] == 'c')
		print = ft_char(print, (char)va_arg(args, int));
	if (str[i] == '%' && str[i + 1] == '%')
		print = ft_char(print, '%');
	else if (str[i] == '%' && str[i + 1] == 's')
		print = ft_string(print, (char *)va_arg(args, char *));
	else if (str[i] == '%' && str[i + 1] == 'u')
		print = ft_unsigned(print, (unsigned int)va_arg(args, unsigned int));
	else if (str[i] == '%' && str[i + 1] == 'p')
		print = ft_pointer(print, (void *)va_arg(args, void *));
	else if (str[i] == '%' && str[i + 1] == 'x')
		print = ft_hexa_lowercase(print, (unsigned int)va_arg(args,
					unsigned int), 1);
	else if (str[i] == '%' && str[i + 1] == 'X')
		print = ft_hexa_uppercase(print, (unsigned int)va_arg(args,
					unsigned int), 1);
	else if (str[i] == '%' && (str[i + 1] == 'd' || str[i + 1] == 'i'))
		print = ft_decimal(print, (int)va_arg(args, int), 1);
	return (print);
}

-----------------------------7838653444561516782759587220--
POST /uploads/post.html HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:132.0) Gecko/20100101 Firefox/132.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
Content-Type: multipart/form-data; boundary=---------------------------7838653444561516782759587220
Content-Length: 2425
Origin: http://localhost:8080
Connection: keep-alive
Referer: http://localhost:8080/
Sec-Fetch-Dest: empty
Sec-Fetch-Mode: cors
Sec-Fetch-Site: same-origin
Priority: u=0

-----------------------------7838653444561516782759587220
Content-Disposition: form-data; name="fileUpload[]"; filename="check_functions.c"
Content-Type: text/x-csrc

/* **********************************************************