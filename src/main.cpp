/*test ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:17:15 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/29 20:23:51 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "logger.hpp"
#include "Server.hpp"
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
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

		//cout << *http << endl;

		//http->start();

		cout << http->getServerByListen("127.0.0.1:8080") << endl;;
		cout << http->getServerByListen("127.0.0.3:8080") << endl;;
		cout << http->getServerByListen("localhost") << endl;;
		cout << http->getServerByListen("127.0.0.2:8080") << endl;;
		cout << http->getServerByListen("teste") << endl;;

	} catch (std::exception &exception) {

		logger::error(exception.what());

		delete http;

		return EXIT_FAILURE;
	}

	delete http;
	
	return EXIT_SUCCESS;
}
