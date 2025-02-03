#include "Http.hpp"
#include "logger.hpp"
#include "Server.hpp"
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <dirent.h>

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
