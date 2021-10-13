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
	
	private:
        bool _autoindex;
		std::vector<std::string> _index;
        std::vector<std::string> _fastcgi_param;
        std::vector<std::string> _fastcgi_pass;

};

#endif