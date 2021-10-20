#include "prototype.hpp"
#include <vector>

void printserver(Server &a);

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
	// // socket
	return handle_socket(servers);
	// return 0;
}


void printserver(Server &a)
{
	std::cout << "---------- SERVER ----------" << "\n" << "\n";
	std::cout << "> port : ";
	std::vector<Port> b = a.get_port();
	std::vector<Port>::iterator it = b.begin(), ite = b.end();
	while (it != ite) {
		std::cout << "[ " << it->port << ", ";
		if (it->ipv4)
			std::cout << "ipv4";
		else
			std::cout << "ipv6";
		std::cout << "]";
		if (it + 1 != ite)
			std::cout << ", ";
		++it;  
	}
	std::cout << "\n";
	if (a.get_default_server() == true)
		std::cout << "> default_server is on" << "\n";
	if (a.get_client_size() > 0)
		std::cout << "> client_max_body_size : " << a.get_client_size() << "\n";
	std::map<int, std::string> c = a.get_error_page();
	if (c.size() > 0)
		std::cout << "> error_page : " << std::endl;
	std::map<int, std::string>::iterator it1 = c.begin(), ite1 = c.end();
	while (it1 != ite1) {
		std::cout << "\t- " << it1->first << " " << it1->second << "\n";
		++it1;
	}
	std::map<std::string, Location> d = a.get_all_location();
	if (d.size() > 0)
		std::cout << "Location : " << "\n";
	std::map<std::string, Location>::iterator it2 = d.begin(), ite2 = d.end();
	while (it2 != ite2) {
		std::cout << "  -> path " << it2->first << "\n";
		if (it2->second.get_autoindex() == true)
		std::cout << "\t- autoindex is on " << "\n"; 
		if (it2->second.get_root() != "")
			std::cout << "\t- root : " << it2->second.get_root() << "\n"; 
		std::vector<std::string> f = it2->second.get_index();
		if (f.size() > 0) {
			std::cout << "\t- index : ";
			printvector(f);
		}
		f = it2->second.get_param();
		if (f.size() > 0) {
			std::cout << "\t- param : ";
			printvector(f);
		}
		f = it2->second.get_pass();
		if (f.size() > 0) {
			std::cout << "\t- pass : ";
			printvector(f);
		}
		std::pair<int, std::string> g = it2->second.get_return();
		if (g.first >= 0)
			std::cout << "\t- return : " << g.first << " " << g.second << "\n";
		std::cout << "\n"; 
		++it2;
	}


}

