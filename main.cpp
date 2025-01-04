/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:17:15 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/04 19:49:59 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <exception>
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

	std::set<Server> servers;

	try {

		if (argc == 1)
			configuration_file = "./configuration/default.conf";
		else if (argc == 2)
			configuration_file = argv[1];
		else
		 	throw std::runtime_error("");

		parserConfiguration(configuration_file);

		cout << "WebReq: " << configuration_file << endl;

	} catch (std::exception &exception) {

		cerr << exception.what() << endl;

		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
