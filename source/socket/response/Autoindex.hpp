#ifndef _AUTOINDEX_HPP_
# define _AUTOINDEX_HPP_

# include <iostream>
# include <dirent.h>
# include <vector>
# include "../proto.hpp"
# include "Response.hpp"
# include <cstring>

class Autoindex
{
    public :
        // Canonical
        Autoindex(std::string & path_file, std::string & request_path, std::string const & host, int const port);
        Autoindex(Autoindex const & src);
        virtual ~Autoindex();
        Autoindex & operator=(Autoindex const & rhs);

		std::string get_content(void);

    private:
		std::string 	_path_file;
		std::string 	_request_path;
		std::string const &   _host;
		int const 		_port;
		std::string		_body;
		std::string 	_dirname;
		std::vector<dirent*> _directory;
		std::vector<dirent*> _file;

		void fill_lists(void);
		std::string information(int len_name, std::string & filename, bool const & is_dir);
		std::string get_date(std::string & filename);
		std::string create_element(std::string name_file, bool const is_dir);
		void create_list_element(std::vector<dirent*> & list, bool const is_dir);
		std::string current_host(void);
		std::string format_name(std::string & name, bool const & is_dir);

    

};

#endif /* _AUTOINDEX_HPP_ */