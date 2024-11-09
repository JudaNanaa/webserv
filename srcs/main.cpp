/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/11/08 20:34:40 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/Parser.hpp"
#include <exception>
#include <vector>

int main(void) {
	Server server;
  std::vector<Server> servVec;
  	try {
		servVec = Pars::parse("configExample.conf");
	} catch (std::exception &e) {
		std::cout << "Error : " << e.what() << std::endl;
		return 1;
	}
  std::cout << "PARSING OK!" << std::endl;
  std::cout << "test post : " << servVec[0]._data->_port << std::endl;
	server.signalHandle();
	server.init();
	server.run();
	std::cout << "ok" << std::endl;
	return 0;
}
