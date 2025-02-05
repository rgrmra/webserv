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
	//struct dirent *file;
	//DIR *root = opendir("/proc/self/fd/");
	//if (!root)
	//	exit(signal);
	//while (1)
	//{
	//	file = readdir(root);
	//	if (!file)
	//		break ;
	//	int fd = atoi(file->d_name);
	//	if (fd > -1 && fd < 1024)
	//		close(atoi(file->d_name));
	//}
	//closedir(root);
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
