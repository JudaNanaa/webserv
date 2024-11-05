/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 00:05:09 by madamou           #+#    #+#             */
/*   Updated: 2024/11/05 00:24:55 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/Server.hpp"

int main(void) {
	Server server;

	server.init();
	server.run();
	std::cout << "ok" << std::endl;
	return 0;
}