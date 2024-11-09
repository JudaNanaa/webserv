/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/11/09 14:50:50 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/GlobalData.hpp"
#include "../includes/Server.hpp"
#include "../includes/Parser.hpp"
#include <exception>
#include <vector>

int main(void) {
	GlobalData data;
	Server server;
	
  	std::vector<Server> servVec;
  	try {
		servVec = Pars::parse("easyParsTest.conf");
	} catch (std::exception &e) {
		std::cout << "Error : " << e.what() << std::endl;
		return 1;
	}
	std::cout << "PARSING OK!" << std::endl;
	std::cout << "test post : " << servVec[0]._data->_port << std::endl;
	try {
		data.runServers(servVec);
	} catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	std::cout << "ok" << std::endl;
	return 0;
}
