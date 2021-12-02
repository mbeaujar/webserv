#include "prototype.hpp"
#include <vector>
#include <algorithm>

void printserver(Server &a);
void check_doublon_port(std::vector<Server> &servers);
void debug(std::vector<Server> &servers);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "number of arguments invalid"
				  << "\n";
		return EXIT_FAILURE;
	}
	std::vector<Server> servers;
	try
	{
		servers = parser(argv[1]);
		check_doublon_port(servers);
		// debug(servers);
		handle_socket(servers);
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: [emerg] " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

bool is_inf(Port &a, Port &b) { return a < b; }

void check_doublon_port(std::vector<Server> &servers)
{
	std::vector<Server>::iterator it = servers.begin(), ite = servers.end();
	std::vector<Port> all;
	std::vector<Port>::iterator begin, end;

	while (it != ite)
	{
		std::vector<Port> &tmp = it->get_port();
		if (tmp.size() == 0)
			throw std::invalid_argument("Server without port");
		begin = tmp.begin();
		end = tmp.end();
		while (begin != end)
		{
			all.push_back(*begin);
			++begin;
		}
		++it;
	}
	if (all.size() == 0)
		throw std::invalid_argument("GROS PROBLEME");
	std::sort(all.begin(), all.end(), is_inf);
	begin = all.begin();
	end = all.end();
	while (begin != end)
	{
		std::vector<Port>::iterator tmp1 = begin + 1;
		if (tmp1 == end)
			break;
		if (*tmp1 == *begin)
			throw std::invalid_argument("There is a doublon for the port: " + to_string(tmp1->port));
		++begin;
	}
}

void debug(std::vector<Server> &servers)
{
	std::vector<Server>::iterator it = servers.begin(), ite = servers.end();
	while (it != ite)
	{
		printserver(*it);
		++it;
	}
}


void printserver(Server &a)
{
	std::cout << "---------- SERVER ----------"
			  << "\n"
			  << "\n";
	std::cout << "> port : ";
	std::vector<Port> b = a.get_port();
	std::vector<Port>::iterator it = b.begin(), ite = b.end();
	while (it != ite)
	{
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
		std::cout << "> default_server is on"
				  << "\n";
	//if (a.get_client_size() > 0)
	//	std::cout << "> client_max_body_size : " << a.get_client_size() << "\n";
	std::map<int, std::string> c = a.get_error_page();
	if (c.size() > 0)
		std::cout << "> error_page : " << std::endl;
	std::map<int, std::string>::iterator it1 = c.begin(), ite1 = c.end();
	while (it1 != ite1)
	{
		std::cout << "\t- " << it1->first << " " << it1->second << "\n";
		++it1;
	}
	std::map<std::string, Location> d = a.get_all_location();
	if (d.size() > 0)
		std::cout << "Location : "
				  << "\n";
	std::map<std::string, Location>::iterator it2 = d.begin(), ite2 = d.end();
	while (it2 != ite2)
	{
		std::cout << "  -> path " << it2->first << "\n";
		if (it2->second.get_autoindex() == true)
			std::cout << "\t- autoindex is on "
					  << "\n";
		if (it2->second.get_root() != "")
			std::cout << "\t- root : " << it2->second.get_root() << "\n";
		std::vector<std::string> f = it2->second.get_index();
		if (f.size() > 0)
		{
			std::cout << "\t- index : ";
			printvector(f);
		}
		s_method z = it2->second.get_methods();
		std::cout << "\t- method : ";
		if (z.m_get == true)
			std::cout << "GET";
		if (z.m_post == true)
		{
			if (z.m_get == true)
				std::cout << ", ";
			std::cout << "POST";
		}
		if (z.m_delete == true)
		{
			if (z.m_post == true || z.m_get == true)
				std::cout << ", ";
			std::cout << "DELETE";
		}
		std::cout << std::endl;
		if (it2->second.get_path_cgi() != "")
			std::cout << "\t- fastcgi: " << it2->second.get_path_cgi() << std::endl;
		if (it2->second.get_max_body() != -1)
			std::cout << "\t- max_body: " << it2->second.get_max_body() << std::endl;
		std::pair<int, std::string> g = it2->second.get_return();
		if (g.first >= 0)
			std::cout << "\t- return : " << g.first << " " << g.second << "\n";
		std::cout << "\n";
		++it2;
	}
}
