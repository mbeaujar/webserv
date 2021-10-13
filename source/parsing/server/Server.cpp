#include "Server.hpp"

/**
 * @brief Construct a new Server::Server object
 * 
 */
Server::Server()
	: _default(false),
	  _autoindex(false),
	  _root(),
	  _name(),
	  _error_page() {}


/**
 * @brief Construct a new Server::Server object
 * 
 * @param src 
 */
Server::Server(Server const & src)
	: _default(src._default),
	  _autoindex(src._autoindex),
	  _root(src._root),
	  _name(src._name),
	  _error_page(src._error_page) {}


/**
 * @brief Destroy the Server::Server object
 * 
 */
Server::~Server() {}

/**
 * @brief operator assignation
 * 
 * @param rhs 
 * @return Server& 
 */
Server & Server::operator=(Server const & rhs) {
    if (this == &rhs)
        return *this;
	_default = rhs._default;
	_autoindex = rhs._autoindex;
	_root = rhs._root;
	_name = rhs._name;
	_error_page = rhs._error_page;
    return *this;
}

/**
 * @brief return true if the Server is the Server by default
 * 
 * @return true 
 * @return false 
 */
bool Server::is_default() const { return _default; }

/**
 * @brief return true if autoindex is on
 * 
 * @return true 
 * @return false 
 */
bool Server::get_autoindex() const { return _autoindex; }

/**
 * @brief search {name} in the Server_name
 * 
 * @param name string to find
 * @return true 
 * @return false 
 */
bool Server::find_name(std::string const &name) const {
	std::vector<std::string>::const_iterator it, ite;

	ite = _name.end();
	for (it = _name.begin(); it != ite; ++it) {
		if (*it == name)
			return true;
	}
	return false;
}

/**
 * @brief Search the url in the error_page
 * 
 * @param error_code code affiliate to an url
 * @return std::string 
 */
std::string Server::find_error(int const & error_code) const {
    std::map<int, std::string>::const_iterator error_msg = _error_page.find(error_code);
    if (error_msg == _error_page.end())
        return "";
    return error_msg->second;
}

/**
 * @brief Set the default object
 * 
 * @param is_default 
 */
void Server::set_default(bool is_default) { _default = is_default; }

/**
 * @brief Set the autoindex object
 * 
 * @param autoindex 
 */
void Server::set_autoindex(bool autoindex) { _autoindex = autoindex; }

/**
 * @brief Set the root object
 * 
 * @param root 
 */
void Server::set_root(std::string const &root) { _root = root; }

/**
 * @brief Add a new Server_name
 * 
 * @param new_name Server_name
 */
void Server::add_Server_name(std::string const &new_name) { _name.push_back(new_name); }

/**
 * @brief Add a new error_page
 * 
 * @param new_error pair<int, std::string>
 */
void Server::add_error_page(std::pair<int, std::string> const &new_error) { _error_page.insert(new_error); }
/**
 * @brief
 *
 * @param
 */
std::vector<Location>       Server::get_location() { return _location; }

void Server::cut_location(std::string file)
{
	int i = 0;
	int debut = 0;
	int fin = 0;
	std::string block = "";

	while (file[i] != '\0')
	{
		block = "";
		if (file[i] == 'l' && 
		file[i + 1] && file[i + 1] == 'o' && 
		file[i + 2] && file[i + 2] == 'c' && 
		file[i + 3] && file[i + 3] == 'a' && 
		file[i + 4] && file[i + 4] == 't' && 
		file[i + 5] && file[i + 5] == 'i' && 
		file[i + 6] && file[i + 6] == 'o' && 
		file[i + 7] && file[i + 7] == 'n')
		{
			debut = i;
			while (file[i] && file[i] != '}')
			{
				block += file[i];
				i++;
			}
			if (file[i] == '}')
				fin = i + 1;
			else
				throw std::invalid_argument("Wrong file");
			block += file[i];
			_location.push_back(Location(block));
		}
		i++;
	}
}

