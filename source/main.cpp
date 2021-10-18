#include "prototype.hpp"
#include <vector>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "number of arguments invalid" << "\n";
		return 1;
	}
	std::vector<Server> servers;
	// parsing 
	try {
		servers = parser(argv[1]);
	}
    catch (std::exception &e) {
		std::cout << "webserv: [emerg] " << e.what() << std::endl;
		return 1;
	}
	// debug
	std::vector<Server>::iterator it = servers.begin(), ite = servers.end();
	while (it != ite) {
		printserver(*it);
		++it;
	}
	// socket
	handle_socket(servers);


	return 0;
}

