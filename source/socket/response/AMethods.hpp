#ifndef _AMETHODS_HPP_
# define _AMETHODS_HPP_

# include "../request/Request.hpp"
# include "../../config/server/Server.hpp"
# include "../../config/server/Location.hpp"
# include "../proto.hpp"
# include <iostream>
# include <sstream>

class AMethods 
{
	public:
		AMethods(Server & server, Request & request, int method, int & client_socket);
		AMethods(AMethods const &copy);
		virtual ~AMethods();
		AMethods & operator=(AMethods const & rhs);

		virtual void execute(void) = 0;
		int		search_location(void);
		bool is_method_allowed(void);
		std::string & get_body(void);

	private:
		std::string path_in_common(std::string const & path_location, std::string & path);
		void 		update_path_file(void);
		int		path_to_file(void);
		bool		path_upload(void);

	protected:
		int		 	_method;
		Location*	_location;
		std::string _path_file;
		Server & 	_server;
		Request & 	_request;
		int		&	_client_socket;
		std::string _body;
	
		bool 		is_extension(std::string & path, std::string const & extension);
};

#endif /* _AMETHODS_HPP_ */
