/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/09 17:44:55 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RawBits.hpp"
#include <cstring>
#include <exception>
#include <stdexcept>
#include <vector>

int	ft_is_in_charset(char c, char *set)
{
	return (c != '\0' && std::strchr(set, c) != NULL);
}

static char	*ft_norminette(char const *s, int i, int j)
{
	int		k;
	char	*str;

	k = 0;
	str = new char[j + 1];
	if (str == NULL)
		return (NULL);
	while (k < j)
	{
		str[k] = s[i - j + k];
		k++;
	}
	str[k] = '\0';
	return (str);
}

void	free_split(std::vector<char *> split) {
	std::vector<char *>::iterator	end = split.end();

	for (std::vector<char *>::iterator it = split.begin(); it != end; it++) {
		delete *it;
	}
}

static int	ft_split_words(std::vector<char *> split, char const *s, char *c)
{
	int	i;
	int	j;

	i = 0;
	while (s[i])
	{
		while (ft_is_in_charset(s[i], c) && s[i])
			i++;
		j = 0;
		while (!ft_is_in_charset(s[i], c) && s[i])
		{
			i++;
			j++;
		}
		if (!ft_is_in_charset(s[i - 1], c))
		{
			split.push_back(ft_norminette(s, i, j));
			if (split.back() == NULL)
				return (free_split(split), 1);
		}
	}
	return (0);
}

std::vector<char *>	RawBits::split(char *s, char *c)
{
	std::vector<char *>	split;

	if (!s)
		return (split);
	if (ft_split_words(split, s, c) == 1)
		throw std::runtime_error("Split failed");
	return (split);
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}