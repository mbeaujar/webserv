#ifndef _LOCATION_HPP_
# define _LOCATION_HPP_

# include "s_method.hpp"

# include <iostream>
# include <vector>
# include <map>

# define DELETE 0
# define GET    1
# define POST   2

class Location 
{
    public:
        Location();
        Location(Location const & copy);
        virtual ~Location();
        Location &operator=(Location const &copy);

		void adding_index(std::string const & index);
        void adding_method(int nb);

		void						set_max_body(int const & max_body);
		void						set_root(std::string const & root);	
		void						set_autoindex(bool const & autoindex);
		void						set_upload(std::string const & upload);
		void						set_cgi(std::string const & ext, std::string const & path);
		void						set_return(int const & code, std::string const & path);

		int	&						get_max_body();
		bool &						get_autoindex();
		std::string &				get_root();
		s_method &					get_methods();
        bool 						get_method(int nb);
		std::string	&				get_upload();
		std::vector<std::string> &	get_index();
		std::pair<int, std::string>	& get_return();
		std::map<std::string, std::string> & get_cgi();

		std::string find_path_cgi(std::string const & ext);

		bool  find_index(std::string const & index);
	
	private:
        bool 								_autoindex;
		int									_max_body;
		std::string							_root;
		std::vector<std::string> 			_index;
        s_method	                		_method;
		std::string 						_upload;
		std::map<std::string, std::string>	_cgi;
		std::pair<int, std::string>			_return;

};

#endif /* _LOCATION_HPP_  */