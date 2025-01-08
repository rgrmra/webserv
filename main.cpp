/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:17:15 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/08 18:53:57 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "./src/parsers/Configuration/Server.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

	string configuration_file;

	try {

		if (argc == 1)
			configuration_file = "./configurations/default.conf";
		else if (argc == 2)
			configuration_file = argv[1];
		else
		 	throw std::runtime_error("");

		Server servers = Server(configuration_file);

		cout << "WebReq: " << configuration_file << endl;
		cout << servers << endl;

	} catch (std::exception &exception) {

		cerr << exception.what() << endl;

		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
