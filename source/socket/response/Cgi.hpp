#ifndef _CGI_HPP_
# define _CGI_HPP_

# include "../request/Request.hpp"
# include "../proto.hpp"
// # include "../../config/server"
# include <iostream>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# define FREE_LINEAR -1

class Cgi 
{
	public:
        Cgi();
        Cgi(Cgi const & copy);
        virtual ~Cgi();
        Cgi & operator=(Cgi const & rhs);

		std::string & execute(Request & request, int method, int & client_socket, std::string & path_in);
		
 	private:
		int 			_pid;
		int				_fd_in;
		int				_fd_out;
		char**			_argv;
		char**			_envp;
		std::string		_content;
		std::string		_path_cgi;
		std::string 	_query_string;
		std::string		_path_file_in;
		std::string		_path_file_out;

		void free_tab(char **tab, int len);
		char* strdup(std::string const & s1);
		int create_argv(void);
		int create_envp(int & method);
		int create_file(std::string name);
		std::string error(Request & request, std::string reason);
		std::string & parse(Request & request, int & client_socket);
		std::string parse_cgi(Request &request, std::string response);
		void parse_header(Request &request, std::string header);
		void parse_status(Request &request, std::string error);
		int skip_line(std::string line, int i);
		int header_size(std::string & content);
};

#endif /* _CGI_HPP_ */


