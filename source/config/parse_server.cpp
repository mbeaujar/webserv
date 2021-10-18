#include <iostream>
#include "server/Server.hpp"
#include "../prototype.hpp"

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

std::vector<Server> parse_server(std::string file)
{
	std::vector<Server> lst;

	for (int i = 0; file[i]; i++)
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);	
		if (file[i] && file.compare(i, 6, "server") == 0) 
		{
			i += 6;
			while (file[i] && file[i] != '{') { // skip space + comment
				if (isspace(file[i]))
					i = skip_space(file, i);
				else if (file[i] == '#')
					i = skip_comment(file, i);
				else 
					throw std::invalid_argument("Invalid number of arguments in \"server\"");
			}
			if (file[i] == '{') { // add a new server
				int skip = skip_bracket(file, i);
				std::string server = file.substr(i, skip - (i - 1));
				// std::cout << "|" << server << "|" << std::endl;
				lst.push_back(config_server(server));
				i = skip;
			}
			else
				throw std::invalid_argument("unknow directive, expected '{' after \"server\"");
		}
		else if (file[i] && !isspace(file[i]) && file[i] != '#') {
			if (file[i] == '}' || file[i] == '{')
				throw std::invalid_argument("unexpected \"{\" or \"}\"");
			throw std::invalid_argument("unknow directive between server");
		}
	}
	return lst;
}