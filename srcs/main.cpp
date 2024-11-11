/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/11/09 20:25:15 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "class/GlobalData/GlobalData.hpp"
#include "class/Server/Server.hpp"
#include "class/Parser/Parser.hpp"
#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
	GlobalData data;
	
	if (argc != 2) {
		std::cerr << "Need only one argument" << std::endl;
		return 1;
	}
  	std::vector<Server> servVec;
  	try {
		servVec = Pars::parse(argv[1]);
	} catch (std::exception &e) {
		std::cout << "Error : " << e.what() << std::endl;
		return 1;
	}
	std::cout << "PARSING OK!" << std::endl;
	std::vector<Server>::iterator it = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();
	int i = 0;
	while (it != end) {
		std::cout << "server on : http://127.0.0.1:" << servVec[i]._data->_port << std::endl;
		it++;
		i++;
	}
	try {
		data.runServers(servVec);
	} catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	std::cout << "ok" << std::endl;
	return 0;
}
