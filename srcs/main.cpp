/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/11/10 18:39:51 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/GlobalData.hpp"
#include "../includes/Server.hpp"
#include "../includes/Parser.hpp"
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
	try {
		data.runServers(servVec);
	} catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	std::cout << "ok" << std::endl;
	return 0;
}
