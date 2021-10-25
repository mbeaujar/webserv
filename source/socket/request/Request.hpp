#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

// # include "../socket.hpp"
# include <iostream>

# define GZIP 0
# define CHUNKED 1

class Request {
    public:
        Request();
        ~Request();
        Request(Request const & src);
        Request &operator=(Request const & rhs); 

        void			set_method(int const & method);
        void			set_path(std::string const & path);
        void			set_host(std::string const & host);
        void			set_content_length(int len);
        void			set_content_type(std::string const & type);
        void			set_transfer_encoding(int const & t_p);
        void			set_error(std::pair<int, std::string> const & error);

        std::string		get_path() const;
        std::string		get_host() const;
        int				get_method() const;
        int		        get_content_length() const;
        std::string		get_content_type() const;
		int				get_transfer_encoding() const;
        std::pair<int, std::string> 	get_error() const;

    private:
        std::string     _host;
        int           	_method;
        std::string		_path;
        int             _content_length;
        std::string     _content_type;
        int 			_transfer_encoding;// Transfer-Encoding: chunked
		std::pair<int, std::string> 	_error;
};

#endif


