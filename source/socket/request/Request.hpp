#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_

# include <iostream>
# include "../../config/server/Server.hpp"
# include <unistd.h>
# include <sys/socket.h>
# include <stdlib.h>
# define MAX_LEN 8192
# define FIX_BROKEN_PIPE usleep(TIME)

void lower_file(std::string & request);

class Request
{
    public:
        // Canonical
        Request(int & client_socket, Server const & server);
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
        void			            set_query_string(std::string const & query_string);
        void			            set_error(std::pair<int, std::string> const & error);
        void			            set_return(std::pair<int, std::string> const & error);

		// Getters
        int	&			            get_method();
        int	&	                    get_content_length();
		std::string &               get_path() ;
        std::string &               get_date();
        std::string	&	            get_host();
		std::string	&	            get_query_string();
        std::string	&	            get_content_type();
        struct s_method	&           get_methods();
		std::string &         		get_file();
        std::pair<int, std::string> &get_error();
        std::pair<int, std::string> &get_return();

        //Methods
    private:
        int           	            _method;
        int                         _content_length;
        std::string                 _host;
        std::string		            _path;
		std::string		            _file;
		std::string		            _date;
        std::string		            _query_string;
        std::string                 _content_type;
       	struct s_method    		    _methods;

		std::pair<int, std::string> _error;
		std::pair<int, std::string> _return;
        
        int         skip_the_word(std::string & file, int i);
        int         find_query_string(std::string & request, int i);
        void        get_first_line(std::string & request, Server const & server);
        std::string recup_word(std::string & request, int & i);
		char*		read_header(int client_socket);
        void        parse_header(std::string header, Server const & server);
};

#endif /* _REQUEST_HPP_ */


