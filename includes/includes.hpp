/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:18:07 by madamou           #+#    #+#             */
/*   Updated: 2024/11/09 18:39:12 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#define GET_ 00000001
#define POST_ 00000010
#define DELETE_ 00000100
#define OPTIONS_ 00001000

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <algorithm>


extern bool g_running;

#endif