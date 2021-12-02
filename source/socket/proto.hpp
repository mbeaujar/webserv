#ifndef _PROTO_HPP_
# define _PROTO_HPP_

# include <iostream>
# include <sstream>

# define ISREDIRECT(x) (x != -1) // No redirection is -1

int				remove_file(char const * path);
int				skip_space(std::string & file, int i);
int				recup_nb(std::string & file, int & i);
void			print_request(Request &a);
bool			is_directory(std::string path);
bool			file_exist(std::string filename);
std::string		path_dir(std::string path);
std::string		path_filename(std::string path);
std::string &	get_file_content(std::string filename);

template <typename tostring>
std::string to_string(tostring n)
{
	std::ostringstream ss;
	ss << n;
	return ss.str();
}

#endif  /* _PROTO_HPP_ */
