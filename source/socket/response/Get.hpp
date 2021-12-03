#ifndef _GET_HPP_
# define _GET_HPP_

# include "AMethods.hpp"
# include "Response.hpp"
# include "Autoindex.hpp"
# include "../proto.hpp"
# include "Cgi.hpp"

class Get : public AMethods
{
    public:
        // Canonical
        Get(Server & server, Request & request, int & client_socket);
        Get(Get const & copy);
        virtual ~Get();
        Get & operator=(Get const & rhs);

        void execute(void);
	
	private:


};

#endif /* _GET_HPP_ */