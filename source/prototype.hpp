#ifndef __PROTOTYPE_HPP__
# define __PROTOTYPE_HPP__

# include "config/server/Server.hpp"
# include "config/server/Location.hpp"
# include <vector>
# include <sstream>

# define DELETE 0
# define GET    1
# define POST   2

# ifndef LIMIT
#  define LIMIT -1
# endif

std::string get_last_modified(std::string path);

// -------------------------------- config

std::vector<Server>	parser(const char *filename);

// -------------- config/server

std::vector<Server> parse_server(std::string file);
Location 			parse_location(std::string file);

// -------------- config/directives

int					skip_word_request(std::string file, int i);
int 				skip_word_exception(std::string file, int i);
int					skip_bracket(std::string file, int i);
int					skip_comment(std::string file, int i);
int					skip_space(std::string file, int i);
int					skip_word(std::string file, int i);
int					parse_autoindex(std::string file, int i, Location &a);
int 				parse_client_size(std::string file, int i, Server &a);
int					parse_error_page(std::string file, int i, Server &a);
int					parse_error_page(std::string file, int i, Server &a);
int					parse_elements(std::string file, int i, Server &a);
int 				parse_return(std::string file, int i, Location &a);
int                 parse_method(std::string file, int i, Location &a);
int 				parse_index(std::string file, int i, Location &a);
int 				parse_listen(std::string file, int i, Server &a);
int 				parse_root(std::string file, int i, Location &a);
int 				parse_upload(std::string file, int i, Location &a);
int 				parse_fastcgi(std::string file, int i, Location &a);
int 				parse_fastcgi_extension(std::string file, int i, Location &a);
int					recup_nb(std::string file, int & i);


// -------------------------------- socket 

int 				handle_socket(std::vector<Server> & servers);
bool		is_directory(std::string path);
bool		file_exist(std::string filename);



template <typename tostring>
std::string to_string(tostring n)
{
	std::ostringstream ss;
	ss << n;
	return ss.str();
}

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

#endif