/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itahri <itahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/05 14:57:47 by itahri           ###   ########.fr       */
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

static int	ft_count_word(char const *s, char *c)
{
	int	i;
	int	cpt;

	cpt = 0;
	i = 0;
	while (s[i])
	{
		while (ft_is_in_charset(s[i], c) && s[i])
			i++;
		while (!ft_is_in_charset(s[i], c) && s[i])
			i++;
		cpt++;
	}
	if (i == 0)
		return (0);
	if (ft_is_in_charset(s[i - 1], c))
		cpt--;
	return (cpt);
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
	int	index;

	i = 0;
	index = 0;
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
		throw std::invalid_argument("Split failed");
	return (split);
}