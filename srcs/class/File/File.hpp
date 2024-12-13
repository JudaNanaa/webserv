/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:36:18 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/13 09:59:22 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/includes.hpp"
#include <cstddef>
#include <fstream>
#include <ostream>

#ifndef FILE_HPP
# define FILE_HPP

class File {
	private:
		std::map<std::string, std::string>		_infos;
	public:
		File();
		~File();
		
		// search a the value of key, throw an std::exception if not found
		const std::string						&get(const std::string &key);
		void									set(const std::string &key, const std::string &value);
		void									clean(void);

};

#endif