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

static void sigexit(int signal) {

	delete http;
	
	exit(signal);
}

int main(int argc, char *argv[]) {

	signal(SIGINT, sigexit);

	int status = EXIT_SUCCESS;

	try {

		if (argc > 2)
			throw std::runtime_error("too many configuration files");

		http = new Http(argv[1] ? argv[1] : "configurations/default.conf");

		cout << *http << endl;

		http->start();

	} catch (std::exception &exception) {

		logger::error(exception.what());

		status = EXIT_FAILURE;
	}

	sigexit(status);
}
