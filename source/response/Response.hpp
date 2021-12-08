#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_

# include "Request.hpp"
# include "Server.hpp"
# include "prototype.hpp"
# include "AMethods.hpp"
# include "Post.hpp"
# include "Get.hpp"
# include "Delete.hpp"

# include <string>
# include <iostream>
# include <sys/stat.h>
# include <cstring>

std::string get_last_modified(std::string & path);

class Response
{
    public:
        Response(int & client_socket, Request & request, Server & server, int & port);
        Response(Response const & copy);
        virtual ~Response();
        Response & operator=(Response const & rhs);
	
		void execute(void);

	private:
		int & _port;
		int &	_client_socket;
		Request & 		_request;
		Server & 	_server;
		std::string _response;
		AMethods* _method;
		size_t		_content_length;
		std::map<std::string, std::string> _mime;
		std::map<int, std::string> 			_error;


		void create_method(int & method);
		void send_response(void);
		std::string get_hour_date(void);
		std::string error_html(void);
        void create_header(void);
        std::string allow_method(void);
		bool set_content_type(std::string content_type, std::map<std::string, int> & accept);
		void check_content_type(std::string & path_file);
		void init_type_mimes(void);
		void init_error(void);
};

#endif /* _RESPONSE_HPP_ */
