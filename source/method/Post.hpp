#ifndef _POST_HPP_
#define _POST_HPP_

#include "AMethods.hpp"
#include "Cgi.hpp"
#include "prototype.hpp"

#include <cstring>
#include <fstream>

#define NO_BOUNDARY 0
#define BOUNDARY 1
#define END_BOUNDARY 2
#define BUFFERSIZE 1024
#define APPEND 1
#define NOAPPEND 0

class Post : public AMethods
{
  public:
    Post(Server &server, Request &request, int &client_socket, int &port);
    Post(Post const &copy);
    virtual ~Post();
    Post &operator=(Post const &rhs);

    virtual void execute(std::map<std::string, std::string> &mime);

  private:
    int _file_fd;
    int _content_length;
    int _ret;
    int _totalsize;
    int _clientmax;
    char *_buffer;
    std::string _file_name;

    int is_boundary(std::string &);
    void read_boundary(void);
    int parse_boundary_header(std::string line);
    int parse_boundary_content_disposition(std::string &line);
    int parse_boundary_content_type(std::string &line);

    int read_buffer(int size);
    int read_endline(void);
    int read_body_child(int msgsize);
    void read_for(void);
    void read_body(void);
    int read_chunked_length(void);
    void read_body_chunked(void);
};

#endif /* _POST_HPP_ */