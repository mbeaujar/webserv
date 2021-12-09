#ifndef _DELETE_HPP_
# define _DELETE_HPP_

# include "AMethods.hpp"
# include "Response.hpp"
# include "prototype.hpp"

class Delete : public AMethods
{
    public:
        Delete(Server & server, Request & request, int & client_socket, int & port);
        Delete(Delete const & copy);
        virtual ~Delete();
        Delete & operator=(Delete const & rhs);

        void execute(std::map<std::string, std::string> & mime);

};

#endif /* _DELETE_HPP_ */
