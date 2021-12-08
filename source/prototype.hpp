#ifndef _PROTOTYPE_HPP_
# define _PROTOTYPE_HPP_

# include "Server.hpp"
# include "Location.hpp"

# include <iostream>
# include <sstream>
# include <vector>

# define DELETE 0
# define GET    1
# define POST   2
# define OTHER  3
# define ISERROR(x) (x >= 400) // 4XX request error, 5xx server error
# define ISREDIRECT(x) (x != -1) // No redirection is -1
# define IS_RANGE_REDIRECT(x) (x >= 300 && x <= 307 && x != 306)

// -------------------------------- config

std::vector<Server>	parser(const char *filename);

// -------------- config/server

Location 			parse_location(std::string file);
std::vector<Server> parse_server(std::string file);
void				printserver(Server &a);

// -------------- config/directives

int 		parse_client_size(std::string file, int i, Location &a);
int			parse_autoindex(std::string file, int i, Location &a);
int			parse_error_page(std::string file, int i, Server &a);
int			parse_error_page(std::string file, int i, Server &a);
int 		parse_cgi_ext(std::string file, int i, Location &a);
int			parse_elements(std::string file, int i, Server &a);
int 		parse_upload(std::string file, int i, Location &a);
int 		parse_return(std::string file, int i, Location &a);
int			parse_method(std::string file, int i, Location &a);
int 		parse_index(std::string file, int i, Location &a);
int 		parse_listen(std::string file, int i, Server &a);
int 		parse_root(std::string file, int i, Location &a);
int 		parse_cgi(std::string file, int i, Location &a);
int 		skip_word_exception(std::string & file, int i);
int			skip_bracket(std::string & file, int i);
int			skip_comment(std::string & file, int i);
int			recup_nb(std::string & file, int & i);
int			skip_space(std::string & file, int i);
int			skip_word(std::string & file, int i);

// -------------------------------- other

std::string	extension(std::string & ext);
std::string	get_file_content(std::string filename);
bool		is_directory(std::string & path);
bool		file_exist(std::string & filename);
int			recup_nb(std::string & file, int & i);
int			skip_space(std::string & file, int i);
int 		skip_line(std::string & line, int i);
int			remove_file(char const * path);


// ------------------- TEMPLATE ---------------------- //

template <typename tostring>
std::string to_string(tostring n)
{
	std::ostringstream ss;
	ss << n;
	return ss.str();
}

#endif  /* _PROTOTYPE_HPP_ */