#ifndef _GET_HPP_
# define _GET_HPP_

#include "AMethods.hpp"

class Get : public AMethods
{
    public:
        // Canonical
        Get();
        Get(Get const & copy);
        virtual ~Get();
        Get & operator=(Get const & rhs);

        // Methods
        std::string execute(Server const & server, Request & request);
	
	private:


};

#endif /* _GET_HPP_ */