#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <iostream>
# include <vector>
# include <map>

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

		void 	adding_port(int const & port, bool const & ipv4);
		void 	adding_error_page(int const & error, std::string const & path);

		void	set_limit_body_size(int const & limit_body_size);
		void 	set_default_server(bool const & default_server);
		
		int	 	get_limit_body_size() const;
		bool  	get_default_server() const;

		bool 	find_port(int const & port) const;

    private:
		std::vector<Port>				_port;
		bool							_default_server;
		std::map<int , std::string>		_error_page;
		int 							_limit_body_size;
		// std::map<std::string, Location> _location;
};

#endif