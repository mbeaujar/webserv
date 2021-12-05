#ifndef _POST_HPP_
# define _POST_HPP_

# include "AMethods.hpp"
# include "Cgi.hpp"
# include "prototype.hpp"

# include <fstream>
# include <cstring>

# define BUFFERSIZE 1024

class Post : public AMethods
{
    public:
        // Canonical
        Post(Server & server, Request & request, int & client_socket);
        Post(Post const & copy);
        virtual ~Post();
        Post & operator=(Post const & rhs);

        // Methods
        int set_file_content(std::string & content);
        virtual void execute(void);

	private:
		int 	_file_fd;
		int 	_content_length;
		int 	_ret;
		int 	_totalsize;
		int 	_clientmax;
		char * 	_buffer;

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