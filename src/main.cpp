/*test ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:17:15 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 22:01:41 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include <Server.hpp>
#include "Http.hpp"
#include "logger.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

	string filename;

	try {

		if (argc == 1)
			filename = "./configurations/default.conf";
		else if (argc == 2)
			filename = argv[1];
		else
		 	throw std::runtime_error("too many configuration files");

		Http servers = Http(filename);

		//cout << servers << endl;

		cout << servers.getServer("meusite.com", "8080") << endl;

	} catch (std::exception &exception) {

		logger::error(exception.what());

		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
