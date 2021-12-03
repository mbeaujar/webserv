#include "Post.hpp"

Post::Post() {}

Post::Post(Post const & src)
{
    *this = src;
}

Post::~Post() {}

Post & Post::operator=(Post const & rhs)
{
    if (this != &rhs)
    {

    }
    return *this;
}

int Post::set_file_content(std::string & filename, std::string &content)
{
	std::ofstream file(filename.c_str(), std::ios_base::app);

	if (file.is_open() == false)
		return EXIT_FAILURE;
	file << content;
	file.close();
	return EXIT_SUCCESS;
}
