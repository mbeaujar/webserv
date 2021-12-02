#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_

# include <string>
# include <iostream>
# include "../request/Request.hpp"
# include "../../config/server/Server.hpp"
# include "../proto.hpp"

class Response
{
    public:
        // Canonical
        Response(int client_socket, Request & request, Server & server);
        Response(Response const & copy);
        virtual ~Response();
        Response & operator=(Response const & rhs);
	
        // Methods
		void execute(void);

        std::string header(Request & request);
        std::string allow_method(Request &request);
		

		// which methods

	private:
		int &			_client_socket;
		Request & 		_request;
		Server & 	_server;

};

#endif /* _RESPONSE_HPP_ */
