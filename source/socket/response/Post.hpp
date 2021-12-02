#ifndef _POST_HPP_
# define _POST_HPP_

# include "AMethods.hpp"

class Post : public AMethods
{
    public:
        // Canonical
        Post();
        Post(Post const & copy);
        virtual ~Post();
        Post & operator=(Post const & rhs);

        // Methods
        int set_file_content(std::string & filename, std::string &content);
        virtual std::string execute(Server const & server, Request & request);

};

#endif /* _POST_HPP_ */