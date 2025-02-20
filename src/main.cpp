#include "Http.hpp"
#include "logger.hpp"
#include "Mime.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

Http *http = NULL;
Mime *mimes = NULL;

static void handle_signal(int signal) {
	
	(void) signal;

	http->stop();
}

int main(int argc, char *argv[]) {

	signal(SIGINT, handle_signal);

	int status = EXIT_SUCCESS;

	try {

		if (argc > 2)
			throw std::runtime_error("too many configuration files");

		http = new Http(argv[1] ? argv[1] : "configurations/default.conf");

		mimes = new Mime("src/parsers/mimes.json");

		cout << *http << endl;

		http->start();

	} catch (std::exception &exception) {

		logger::fatal(exception.what());

		status = EXIT_FAILURE;
	}

	delete http;
	delete mimes;

	return status;
}
