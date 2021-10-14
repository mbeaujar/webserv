#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include <iostream>
# include <vector>

class Location 
{
    public:
        Location();
        Location(Location const &copy);
        virtual ~Location();
        Location &operator=(Location const &copy);

		void adding_index(std::string const & index);
		void adding_param(std::string const & param);
		void adding_pass(std::string const & pass);

		void set_root(std::string const & root);
		void set_autoindex(bool const & autoindex);

		bool get_autoindex() const;
		std::string get_root() const;

		bool find_index(std::string const & index) const;
		bool find_param(std::string const & param) const;
		bool find_pass(std::string const & pass) const;
	
	private:
        bool 						_autoindex;
		std::string					_root;
		std::vector<std::string> 	_index;
        std::vector<std::string> 	_fastcgi_param;
        std::vector<std::string> 	_fastcgi_pass;

};

#endif