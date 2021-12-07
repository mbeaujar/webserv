#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include "Location.hpp"
# include "s_port.hpp"

# include <iostream>
# include <vector>
# include <map>

class Server
{
    public:
        Server();
        Server(Server const &copy);
        virtual	~Server();
        Server&	operator=(Server const &copy);

		int 		get_current_port() const;
		int			get_number_of_port() const;
		bool  		get_default_server() const;
		void		set_current_port(int const & port);
		bool 		find_location(std::string const & path) const;
		void 		set_default_server(bool const & default_server);
		void 		adding_port(int const & port, bool const & ipv4);
		bool 		find_port(int const & port, bool const & ipv4) const;
		void 		adding_error_page(int const & error, std::string const & path);
		void		adding_location(std::string const & path, Location const & location);
		Location 	get_location(std::string const & path) const;
		std::string	find_error(int const & error) const;
		std::vector<s_port>& get_port();
		std::map<int, std::string> get_error_page() const;
		std::map<std::string, Location> get_all_location() const;

    private:
		bool							_default_server;
		int								_current_port;
		std::vector<s_port>				_port;
		std::map<int, std::string>		_error_page;
		std::map<std::string, Location> _location;
};

// ----------- prototype ----------------- //

void				printserver(Server &a);

template <typename T>
void printvector(std::vector<T> &vector)
{
	typename std::vector<T>::iterator it = vector.begin(), ite = vector.end();
	std::cout << "[ ";
	while (it != ite) {
		std::cout << *it;
		if (it + 1 != ite)
			std::cout << ", ";
		++it;
	}
	std::cout << " ]" << "\n";
};

#endif /* _SERVER_HPP_ */