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

        void set_method(int m);
        void set_path(std::string p);
        void set_host(std::string h);

	private:
		std::string		_host;
		int         	_method;
		std::string 	_path;
		// std::string 	_body; // les requêtes POST contiennent un body
		
};

#endif