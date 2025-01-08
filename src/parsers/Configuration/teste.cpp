#include <iostream>
#include "Server.hpp"
int main(void) {


	Server server = Server("/home/rgrmra/webserv/configurations/default.conf");

	std::cout << server << std::endl;

}
