#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

# include "../socket.hpp"
# include <iostream>

class Request {
	public:
        Request();
        ~Request();
        Request(Request const & src);
        Request & operator=(Request const & rhs);

        void 				set_method(int const & method);
        void 					set_path(std::string const & path);
        void 				set_host(std::string const & host);
        int				get_method() const;
        std::string     get_path() const;
        std::string            get_host() const;

	private:
		std::string	_host;
		int         	_method;
		std::string 	_path;
		// std::string 	_body; // les requêtes POST contiennent un body	
};

#endif