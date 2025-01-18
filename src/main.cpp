/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:17:15 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/17 11:02:55 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "parsers/Http.hpp"
#include "services/Logger/Logger.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <set>
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
		 	throw std::runtime_error("");

		Http servers = Http(filename);

		cout << servers << endl;

	} catch (std::exception &exception) {

		Logger::error(exception.what());

		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
