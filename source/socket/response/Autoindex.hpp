#ifndef _AUTOINDEX_HPP_
# define _AUTOINDEX_HPP_

# include <iostream>
# include <dirent.h>
# include <vector>
# include "../proto.hpp"
# include <cstring>

class Autoindex
{
    public :
        // Canonical
        Autoindex(std::string const & path, std::string const & root, std::string const & host, int const & port);
        Autoindex(Autoindex const & src);
        virtual ~Autoindex();
        Autoindex & operator=(Autoindex const & rhs);

    private:
        std::string     _response;

        off_t       file_size(const char *filename);
        std::string put_space(std::string space);
        bool        dirent_comp(struct dirent *a, struct dirent *b);
        std::string put_file_name(std::string file, bool const & is_dir);
        std::string current_host(std::string const & host, int const & port);
        std::string clear_dirname(std::string const & dirname, std::string const & root);
        std::string get_file_size(std::string const & path_to_file, std::string const & file, bool const & is_dir);
        std::string get_data(std::string const & path_to_file, std::string const & filename, std::string const & file);
        std::string listing(std::string const & path, std::string const & root, std::string const & host, int const & port);
        std::string autoindex_on(std::string const & path, std::string const & root, std::string const & host, int const & port);
        std::string put_space_data(std::string const & path_to_file, std::string const & filename, std::string const & file, bool const & is_dir);
        std::string create_element(struct dirent *file, std::string const & dir_name, std::string const & root, bool const & is_dir, std::string const & host, int const & port);
        std::string create_list_element(std::vector<struct dirent *> & list, std::string const & dir_name, std::string const & root, bool const & is_dir, std::string const & host, int const & port);

};

#endif /* _AUTOINDEX_HPP_ */