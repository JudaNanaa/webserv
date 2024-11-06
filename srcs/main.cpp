/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/11/06 17:01:53 by itahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/Parser.hpp"
#include <exception>

int main(void) {
	Server server;

  try {
    Pars::parse("configExample.conf");
  } catch (std::exception &e) {
    std::cout << "Error : " << e.what() << std::endl;
  }
	server.signalHandle();
	server.init();
	server.run();
	std::cout << "ok" << std::endl;
	return 0;
}
