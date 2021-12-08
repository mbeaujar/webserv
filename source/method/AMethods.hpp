#ifndef _AMETHODS_HPP_
# define _AMETHODS_HPP_

# include "Location.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "prototype.hpp"

# include <sys/stat.h>
# include <iostream>
# include <sstream>
# include <fcntl.h>
# include <stdio.h>

int			remove_file(char const * path);
int 		get_port(Server & server, int & client_socket);

class AMethods 
{
	public:
		AMethods(Server & server, Request & request, int method, int & client_socket, int & port);
		AMethods(AMethods const &copy);
		virtual ~AMethods();
		AMethods & operator=(AMethods const & rhs);

		virtual void execute(void) = 0;
		int		search_location(void);
		bool is_method_allowed(void);
		std::string & get_body(void);
		std::string & get_path(void);

	private:
		std::string	path_in_common(std::string const & path_location, std::string & path);
		void 		update_path_file(void);
		int			path_to_file(void);
		bool		path_upload(void);
		void 		create_path(void);

	protected:
		int &		_port;
		int		 	_method;
		Location	_location;
		std::string _path_file;
		Server & 	_server;
		Request & 	_request;
		int		&	_client_socket;
		std::string _body;
	
		bool 		is_extension(std::string & path, std::string const & extension);
};

#endif /* _AMETHODS_HPP_ */
