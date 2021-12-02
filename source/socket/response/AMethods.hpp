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
		// Canonical
		AMethods(int method);
		AMethods(AMethods const &copy);
		virtual ~AMethods();
		AMethods & operator=(AMethods const & rhs);

		// Methods
		bool	method_allowed(int & request_method) const;
		bool		search_location(std::string const & path, Server const & server);
		virtual std::string execute(Server const & server, Request & request) const = 0;

		// Setters
		void			set_method(int const & method);
		void			set_path_file(std::string const & path_file);
		void			set_location(Location const & location);

		// Getters
		int &			get_method();
		Location &		get_location();
		std::string &	get_path_file();
		
	protected:
		int		 	_method;
		Location	_location;
		std::string _path_file;

};

#endif /* _AMETHODS_HPP_ */
