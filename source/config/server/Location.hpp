#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include <iostream>
# include <vector>
# include "s_method.hpp"

# define DELETE 0
# define GET    1
# define POST   2

class Location 
{
    public:
        Location();
        Location(Location const &copy);
        virtual ~Location();
        Location &operator=(Location const &copy);

		void adding_index(std::string const & index);
        void adding_method(int nb);

		void						set_max_body(int const & max_body);
		void						set_root(std::string const & root);
		void						set_autoindex(bool const & autoindex);
		void						set_upload(std::string const & upload);
		void						set_fastcgi(std::string const & fastcgi);
		void						set_fastcgi_ext(std::string const & fastcgi_ext);
		void						set_return(int const & code, std::string const & path);

		int							get_max_body() const;
		bool						get_autoindex() const;
		std::string					get_root() const;
		s_method					get_methods() const;
        bool						get_method(int nb) const;
		std::string					get_upload() const;
		std::string					get_fastcgi() const;
		std::string					get_fastcgi_ext() const;
		std::vector<std::string>	get_index() const;
		std::pair<int, std::string>	get_return() const;

		bool find_index(std::string const & index) const;
	
	private:
        bool 						_autoindex;
		int							_max_body;
		std::string					_root;
		std::vector<std::string> 	_index;
        s_method	                _method;
		std::string 				_upload;
		std::string 				_fastcgi;
		std::string					_fastcgi_ext; // fastcgi_extension
		std::pair<int, std::string> _return;

};

#endif