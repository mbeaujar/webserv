/**
 * @file Server.hpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <iostream>
# include <vector>
# include <map>
# include "Location.hpp"

class Server {
    public:
        Server();
        Server(Server const &copy);
        virtual ~Server();
        Server &operator=(Server const &copy);

		int     get_port() const;
		bool    is_default() const;
		bool    get_autoindex() const;
		std::string get_root() const;

		bool find_name(std::string const &name) const;
		std::string find_error(int const &code) const;


		void set_port(int const &port);
		void set_default(bool is_default);
		void set_autoindex(bool autoindex);
		void set_root(std::string const &root);
		void add_Server_name(std::string const &new_name);
		void add_error_page(std::pair<int, std::string> const &new_error);

    private: 
		int	_port;
		bool _default;
		bool _autoindex;
		std::string _root;
		std::vector<std::string>    _name;
		std::vector<Location>       _location;
		std::map<int, std::string>  _error_page;

};

#endif