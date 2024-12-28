/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/12/28 23:22:31 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "class/GlobalData/GlobalData.hpp"
#include "class/Server/Server.hpp"
#include "class/Parser/Parser.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>
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

int main(int argc, char **argv, char **env) {
	GlobalData data;
	std::vector<Server> servVec;
	
	signalHandle();
	if (argc > 2) {
		std::cerr << "Need only one argument" << std::endl;
		return 1;
	}
	const char *config_path = argc == 2 ? argv[1] : DEFAULT_CONFIG_PATH;
	try {
    	servVec = Pars::parse(config_path, env);
	} catch (std::exception &e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return 1;
	}
	try { 
		data.runServers(servVec);
	}
	catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::vector<Server>::iterator it = servVec.begin();
		std::vector<Server>::iterator end = servVec.end();
		
		while (it is_not end) {
			it->freeAll();
			++it;
		}
		return 1;
	}
	data.closeServers();
	return 0;
}
