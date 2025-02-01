/*test ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:17:15 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/31 20:59:17 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "logger.hpp"
#include "Server.hpp"
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

Http *http = NULL;

void sigint(int signal) {

	delete http;

	exit(signal);
}

int main(int argc, char *argv[]) {

	signal(SIGINT, sigint);

	try {

		if (argc > 2)
			throw std::runtime_error("too many configuration files");

		http = new Http(argv[1] ? argv[1] : "configurations/default.conf");

		cout << *http << endl;

		http->start();

	} catch (std::exception &exception) {

		logger::error(exception.what());

		delete http;

		return EXIT_FAILURE;
	}

	delete http;
	
	return EXIT_SUCCESS;
}
