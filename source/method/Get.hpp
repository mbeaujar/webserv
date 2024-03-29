#ifndef _GET_HPP_
#define _GET_HPP_

#include "AMethods.hpp"
#include "Autoindex.hpp"
#include "Cgi.hpp"
#include "Response.hpp"
#include "prototype.hpp"

class Get : public AMethods
{
  public:
    Get(Server &server, Request &request, int &client_socket, int &port);
    Get(Get const &copy);
    virtual ~Get();
    Get &operator=(Get const &rhs);

    void execute(std::map<std::string, std::string> &mime);
    std::pair<int, std::string> get_redirect(void);

  private:
    bool is_redirect(void);
};

#endif /* _GET_HPP_ */