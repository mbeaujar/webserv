#ifndef _PROTOTYPE_HPP_
# define _PROTOTYPE_HPP_

# include "config/server/Server.hpp"
# include "config/server/Location.hpp"
# include <stdlib.h>
# include <vector>
# include <sstream>

# define DELETE 0
# define GET    1
# define POST   2

// -------------------------------- config

std::vector<Server>	parser(const char *filename);

// -------------- config/server

std::vector<Server> parse_server(std::string file);
Location 			parse_location(std::string file);

// -------------- config/directives

int 				skip_word_exception(std::string & file, int i);
int					skip_bracket(std::string & file, int i);
int					skip_comment(std::string & file, int i);
int					skip_space(std::string & file, int i);
int					skip_word(std::string & file, int i);
int					parse_autoindex(std::string file, int i, Location &a);
int 				parse_client_size(std::string file, int i, Location &a);
int					parse_error_page(std::string file, int i, Server &a);
int					parse_error_page(std::string file, int i, Server &a);
int					parse_elements(std::string file, int i, Server &a);
int 				parse_return(std::string file, int i, Location &a);
int                 parse_method(std::string file, int i, Location &a);
int 				parse_index(std::string file, int i, Location &a);
int 				parse_listen(std::string file, int i, Server &a);
int 				parse_root(std::string file, int i, Location &a);
int 				parse_upload(std::string file, int i, Location &a);
int 				parse_cgi(std::string file, int i, Location &a);
int 				parse_cgi_ext(std::string file, int i, Location &a);
int					recup_nb(std::string & file, int & i);


// -------------------------------- socket 

bool		is_directory(std::string path);
bool		file_exist(std::string filename);


template <typename T>
void printvector(std::vector<T> &vector)
{
	typename std::vector<T>::iterator it = vector.begin(), ite = vector.end();
	std::cout << "[ ";
	while (it != ite) {
		std::cout << *it;
		if (it + 1 != ite)
			std::cout << ", ";
		++it;
	}
	std::cout << " ]" << "\n";
}

#endif /*  _PROTOTYPE_HPP_ */