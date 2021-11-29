#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

// # include "../socket.hpp"
#include "../../config/server/Location.hpp"
# include <iostream>

# define CHUNKED	0
# define GZIP		1

class Request {
    public:
        Request();
        ~Request();
        Request(Request const & src);
        Request &operator=(Request const & rhs);

        void			set_method(int const & method);
        void			set_path(std::string const & path);
        void			set_query_string(std::string const & query_string);
        void			set_host(std::string const & host);
        void			set_content_length(int len);
        void			set_content_type(std::string const & type);
        void			add_transfer_encoding(std::string const & transfer_encoding);
        void			set_error(std::pair<int, std::string> const & error);
        void			set_return(std::pair<int, std::string> const & error);
        void			set_methods(struct s_method & met);
        void			set_date(std::string const & date);
        void			set_version(std::string const & version);
		
		std::string		get_path() const;
		std::string		get_query_string() const;
        std::string		get_host() const;
        int				get_method() const;
        int		        get_content_length() const;
        std::string		get_content_type() const;
		std::vector<std::string>				get_transfer_encoding() const;
        std::pair<int, std::string> 	get_error() const;
        std::pair<int, std::string> 	get_return() const;
        struct s_method		get_methods() const;
        std::string		get_date() const;
        std::string		get_version() const;

    private:
        std::string     _host;
        int           	_method;
        std::string		_path;
		std::string		_version;
        std::string		_query_string;
        int             _content_length;
        std::string     _content_type;
        std::vector<std::string> _transfer_encoding;// Transfer-Encoding: chunked
		std::pair<int, std::string> 	_error;
		std::pair<int, std::string> 	_return;
       	struct s_method    		 _methods;
		std::string		_date;
};

#endif


