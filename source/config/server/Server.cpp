#include "Server.hpp"

Server::Server() : _default_server(false), _current_port(), _port(), _error_page(), _location()
{
}

Server::Server(Server const &copy)
    : _default_server(copy._default_server), _current_port(copy._current_port), _port(copy._port),
      _error_page(copy._error_page), _location(copy._location)
{
}

Server::~Server()
{
}

Server &Server::operator=(Server const &copy)
{
    if (this == &copy)
        return *this;
    _default_server = copy._default_server;
    _current_port = copy._current_port;
    _port = copy._port;
    _error_page = copy._error_page;
    _location = copy._location;
    return *this;
}

void Server::adding_port(int const &port, bool const &ipv4 = true)
{
    s_port a;
    a.port = port;
    a.ipv4 = ipv4;
    a.fd = -1;
    _port.push_back(a);
}

void Server::adding_error_page(int const &error, std::string const &path)
{
    _error_page.insert(std::make_pair(error, path));
}

void Server::adding_location(std::string const &path, Location const &location)
{
    _location.insert(std::make_pair(path, location));
}

void Server::set_current_port(int const &port)
{
    _current_port = port;
}

int Server::get_current_port() const
{
    return _current_port;
}

void Server::set_default_server(bool const &default_server)
{
    _default_server = default_server;
}

std::map<int, std::string> Server::get_error_page() const
{
    return _error_page;
}

int Server::get_number_of_port() const
{
    return _port.size();
}

bool Server::get_default_server() const
{
    return _default_server;
}

std::vector<s_port> &Server::get_port()
{
    return _port;
}

bool Server::find_port(int const &port, bool const &ipv4) const
{
    std::vector<s_port>::const_iterator it = _port.begin(), ite = _port.end();

    while (it != ite)
    {
        if (it->port == port && it->ipv4 == ipv4)
            return true;
        ++it;
    }
    return false;
}

std::string Server::find_error(int const &error) const
{
    std::map<int, std::string>::const_iterator search;

    search = _error_page.find(error);
    if (search != _error_page.end())
        return search->second;
    return "";
}

bool Server::find_location(std::string const &path) const
{
    std::map<std::string, Location>::const_iterator it = _location.find(path);

    if (it != _location.end())
        return true;
    return false;
}

std::map<std::string, Location> Server::get_all_location() const
{
    return _location;
}

Location Server::get_location(std::string const &path) const
{
    return _location.find(path)->second;
}

void printserver(Server &a)
{
    std::cout << "---------- SERVER ----------"
              << "\n"
              << "\n";
    std::cout << "> port : ";
    std::vector<s_port> b = a.get_port();
    std::vector<s_port>::iterator it = b.begin(), ite = b.end();
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
        std::cout << "\t- upload: " << it2->second.get_upload() << std::endl;
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
        std::map<std::string, std::string> &cgi = it2->second.get_cgi();
        std::map<std::string, std::string>::iterator cgi1 = cgi.begin(), cgi2 = cgi.end();
        std::cout << "\t-> cgi" << std::endl;
        while (cgi1 != cgi2)
        {
            std::cout << "\t\t- ext  : " << cgi1->first << std::endl;
            std::cout << "\t\t- path : " << cgi1->second << std::endl;
            ++cgi1;
        }
        std::pair<int, std::string> g = it2->second.get_return();
        if (g.first >= 0)
            std::cout << "\t- return : " << g.first << " " << g.second << "\n";
        std::cout << "\n";
        ++it2;
    }
}
