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
        virtual void execute(void);

};

#endif /* _POST_HPP_ */