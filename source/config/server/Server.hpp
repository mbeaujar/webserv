#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <iostream>
# include <vector>
# include <map>
# include "Location.hpp"

struct Port {
	int port;
	bool ipv4;
};

class Server {
    public:
        Server();
        Server(Server const &copy);
        virtual	~Server();
        Server&	operator=(Server const &copy);

		void 		adding_port(int const & port, bool const & ipv4);
		void 		adding_error_page(int const & error, std::string const & path);
		void		adding_location(std::string const & path, Location const & location);

		void		set_client_size(int const & limit_body_size);
		void 		set_default_server(bool const & default_server);
		
		int	 		get_client_size() const;
		bool  		get_default_server() const;
		Location 	get_location(std::string const & path) const;

		bool 		find_port(int const & port, bool const & ipv4) const;
		std::string	find_error(int const & error) const;
		bool 		find_location(std::string const & path) const;

    private:
		int 							_client_size;
		bool							_default_server;
		std::vector<Port>				_port;
		std::map<int , std::string>		_error_page;
		std::map<std::string, Location> _location;
};

#endif