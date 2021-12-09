#ifndef _POST_HPP_
# define _POST_HPP_

# include "AMethods.hpp"
# include "Cgi.hpp"
# include "prototype.hpp"

# include <fstream>
# include <cstring>

# define BUFFERSIZE 1024
# define APPEND		1
# define NOAPPEND	0

class Post : public AMethods
{
    public:
        // Canonical
        Post(Server & server, Request & request, int & client_socket, int & port);
        Post(Post const & copy);
        virtual ~Post();
        Post & operator=(Post const & rhs);

        // Methods
        virtual void execute(std::map<std::string, std::string> & mime);

	private:
		int 	_file_fd;
		int 	_content_length;
		int 	_ret;
		int 	_totalsize;
		int 	_clientmax;
		char * 	_buffer;
		std::string _file_name;

		int read_buffer(int size);
		int read_endline(void);
		int read_body_child(int msgsize);
		int read_chunk_child(int msgsize);
		void read_for(int (Post::*)(int));
		void read_body(void);
		int read_chunked_length(void);
		void read_body_chunked(void);

};

#endif /* _POST_HPP_ */