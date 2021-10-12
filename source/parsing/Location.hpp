/**
 * @file Location.hpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include <iostream>
# include <vector>

class Location 
{
    public:
        Location();
        Location(Location const &copy);
        ~Location();
        Location &operator=(Location const &copy);
        void cut_location(std::string file);
        void parse_location(void);


        std::string location_block;
        bool autoindex;
        std::vector<std::string> index;
        std::vector<std::string> fastcgi_param;
        std::string fastcgi_pass;
};

#endif