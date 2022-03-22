#ifndef _AMETHODS_HPP_
#define _AMETHODS_HPP_

#include "Location.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "prototype.hpp"

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>

int remove_file(char const *path);
int get_port(Server &server, int &client_socket);

class AMethods
{
  public:
    AMethods(Server &server, Request &request, int method, int &client_socket, int &port);
    AMethods(AMethods const &copy);
    virtual ~AMethods();
    AMethods &operator=(AMethods const &rhs);

    virtual void execute(std::map<std::string, std::string> &mime) = 0;
    int search_location(void);
    bool is_method_allowed(void);
    std::string &get_body(void);
    std::string &get_path(void);
    int set_file_content(std::string &path_file, std::string &content, bool is_app);
    bool path_upload(void);

  private:
    std::string path_in_common(std::string const &path_location, std::string &path);
    void update_path_file(void);
    int path_to_file(void);
    void create_path(void);

  protected:
    int &_port;
    int _method;
    Location _location;
    std::string _path_file;
    Server &_server;
    Request &_request;
    int &_client_socket;
    std::string _body;

    bool is_extension(std::string &path, std::string const &extension);
};

#endif /* _AMETHODS_HPP_ */
