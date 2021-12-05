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
        Response(int & client_socket, Request & request, Server & server);
        Response(Response const & copy);
        virtual ~Response();
        Response & operator=(Response const & rhs);
	
		void execute(void);

	private:
		int &	_client_socket;
		Request & 		_request;
		Server & 	_server;
		std::string _response;
		AMethods* _method;
		size_t		_content_length;


		void create_method(int & method);
		void send_response(void);
		std::string get_hour_date(void);
		std::string error_html(void);
        void create_header(void);
        std::string allow_method(void);
};

#endif /* _RESPONSE_HPP_ */
