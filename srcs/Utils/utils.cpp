/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/14 14:26:41 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../class/RawBits/RawBits.hpp"
#include "../../includes/includes.hpp"
#include <cstddef>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <vector>

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) is_found) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

std::string generateFilename(std::string baseName) {
		std::time_t now = std::time(NULL);
		std::tm* now_tm = std::localtime(&now);

		baseName.erase(0, 1);
		baseName.erase(baseName.size() - 1, 1);
		char timeBuffer[20];
		std::strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d_%H%M%S", now_tm);
		std::ostringstream oss;
		oss << "URIs/uploads/" 
			<< timeBuffer << "_"
			<< baseName;
		return oss.str();
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v\"");
    if (start not_found) return ""; // Chaîne vide si uniquement des espaces
    size_t end = str.find_last_not_of(" \t\n\r\f\v\"");
    return str.substr(start, end - start + 1);
}

void trimn(std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start not_found)
    {
      str.clear();
      return ; // Chaîne vide si uniquement des espaces
    }
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    str.erase(end + 1);
    str.erase(0, start);
}

void formatSlash(std::string &str)
{
	std::size_t found;
	
	while (true)
	{
		found = str.find("//");
		if (found == std::string::npos)
			break;
		str.erase(found,1);
	}
}