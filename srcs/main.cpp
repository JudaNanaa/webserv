/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/11/19 18:12:05 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "class/GlobalData/GlobalData.hpp"
#include "class/Server/Server.hpp"
#include "class/Parser/Parser.hpp"
#include <exception>
#include <iostream>
#include <vector>
#include <csignal>

void ifSignal(int sig) {
	(void)sig;
	g_running = false;
}

void signalHandle(void) {
	signal(SIGINT, ifSignal);
	signal(SIGQUIT, ifSignal);
	signal(SIGTSTP, ifSignal);
}

int main(int argc, char **argv) {
	GlobalData data;
	
	signalHandle();
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
	return 0;
}
