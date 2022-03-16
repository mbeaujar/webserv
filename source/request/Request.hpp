#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_

# include "prototype.hpp"
# include "Server.hpp"

# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
# include <stdlib.h>
# include <cstring>
# include <time.h>

# define MAX_LEN 8192
# define FIX_BROKEN_PIPE usleep(TIME)

void lower_file(std::string & request);
int skip_line(std::string & line, int i);

class Request
{
    public:
        // Canonical
        Request(int & client_socket);
        ~Request();
        Request(Request const & src);
        Request &operator=(Request const & rhs);

        // Setters
        void			            set_content_length(int & len);
        void			            set_method(int const & method);
		void			            set_file(std::string & file);
        void			            set_path(std::string const & path);
        void			            set_host(std::string const & host);
        void			            set_methods(struct s_method & met);
        void			            set_date(std::string const & date);
        void			            set_content_type(std::string const & type);
        void			            set_boundary(std::string const & boundary);
        void			            set_query_string(std::string const & query_string);
        void			            set_error(std::pair<int, std::string> const & error);
        void			            set_return(std::pair<int, std::string> const & error);
        void			            set_accept(std::map<std::string, int> const & accept);
		void						generate_cookie_username(void);
		void						generate_cookie_color(void);

		// Getters
        int	&			            get_method();
        int	&	                    get_content_length();
		std::string &               get_path() ;
        std::string &               get_date();
        std::string	&	            get_host();
		std::string	&	            get_query_string();
        std::string	&	            get_content_type();
        std::string	&	            get_cookie_username();
        std::string	&	            get_cookie_color();
        std::string	&	            get_boundary();
        struct s_method	&           get_methods();
		std::string &         		get_file();
        std::pair<int, std::string> &get_error();
        std::pair<int, std::string> &get_return();
        std::map<std::string, int>  &get_accept();
		bool &                      get_new_client(void);
		std::string & 				get_header();

    private:
        int           	            _method;
        int                         _content_length;
        std::string                 _host;
        std::string		            _path;
		std::string		            _file;
		std::string		            _date;
        std::string		            _query_string;
        std::string                 _content_type;
        std::string                 _boundary;
       	struct s_method    		    _methods;
		std::string 				_cookie_username;
		std::string 				_cookie_color;
		bool 						_new_client;
		std::pair<int, std::string> _error;
		std::pair<int, std::string> _return;
		std::map<std::string, int>  _accept;
		std::string 				_header; // only for cgi
        
		bool 		is_query(std::string & path);
        std::string parse_query(std::string & path);
        int         skip_the_word(std::string & file, int i);
        int        get_first_line(std::string & request);
        std::string recup_word(std::string & request, int & i);
		char*		read_header(int client_socket);
        void        parse_header(std::string header);
		void        print_request(void);
		void 		parse_accept(std::string & content);
		void		parse_cookie(std::string & content);
};

#endif /* _REQUEST_HPP_ */


