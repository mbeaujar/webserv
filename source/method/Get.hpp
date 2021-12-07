#ifndef _GET_HPP_
# define _GET_HPP_

# include "AMethods.hpp"
# include "Response.hpp"
# include "Autoindex.hpp"
# include "prototype.hpp"
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
		std::pair<int, std::string> get_redirect(void);

	private:
		bool is_redirect(void);
};

#endif /* _GET_HPP_ */