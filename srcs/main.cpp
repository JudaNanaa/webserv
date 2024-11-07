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
#include "../includes/Data.hpp"

void signalHandle(void);

int main(void) {
	Data data;

	// Parsing and store servers vector in data.servVect
	data.initServers();
	signalHandle();
	data.runServers();
	// data.closeServers();
	std::cout << "ok" << std::endl;
	return 0;
}