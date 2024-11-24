/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/23 23:05:28 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../class/RawBits/RawBits.hpp"
#include "../../includes/includes.hpp"
#include <cstring>
#include <exception>
#include <stdexcept>
#include <vector>

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
