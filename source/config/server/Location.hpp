#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include <iostream>
# include <vector>

# define DELETE 0
# define GET    1
# define POST   2


struct s_method {
	s_method& operator=(s_method const & rhs);

    bool m_get;
    bool m_post;
    bool m_delete;
};

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
        void adding_method(int nb);

		void set_root(std::string const & root);
		void set_autoindex(bool const & autoindex);
		void set_return(int const & code, std::string const & path);

        bool get_method(int nb) const;
		s_method get_methods() const;
		bool get_autoindex() const;
		std::string get_root() const;
		std::vector<std::string> get_index() const;
		std::vector<std::string> get_param() const;
		std::vector<std::string> get_pass() const;
		std::pair<int, std::string> get_return() const;


		bool find_index(std::string const & index) const;
		bool find_param(std::string const & param) const;
		bool find_pass(std::string const & pass) const;
	
	private:
        bool 						_autoindex;
		std::string					_root;
		std::vector<std::string> 	_index;
        s_method	                _method;
        std::vector<std::string> 	_fastcgi_param;
        std::vector<std::string> 	_fastcgi_pass;
		std::pair<int, std::string> _return;

};

#endif