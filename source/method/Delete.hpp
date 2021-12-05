#ifndef _DELETE_HPP_
# define _DELETE_HPP_

# include "AMethods.hpp"
# include "Response.hpp"
# include "prototype.hpp"

class Delete : public AMethods
{
    public:
        Delete(Server & server, Request & request, int & client_socket);
        Delete(Delete const & copy);
        virtual ~Delete();
        Delete & operator=(Delete const & rhs);

        void execute(void);

};

#endif /* _DELETE_HPP_ */
