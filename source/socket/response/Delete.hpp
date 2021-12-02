#ifndef _DELETE_HPP_
# define _DELETE_HPP_

# include "AMethods.hpp"
# include "Response.hpp"
# include "../proto.hpp"

class Delete : public AMethods
{
    public:
        // Canonical
        Delete();
        Delete(Delete const & copy);
        virtual ~Delete();
        Delete & operator=(Delete const & rhs);

        // Methods
        std::string execute(Server const & server, Request & request);

};

#endif /* _DELETE_HPP_ */
