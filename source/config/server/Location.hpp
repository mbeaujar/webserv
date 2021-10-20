#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include <iostream>
# include <vector>
#include  <map>

# define DELETE 0
# define GET    1
# define POST   2

# define SERVER_SOFTWARE 101
# define QUERY_STRING 102 
# define REQUEST_METHOD 103    
# define CONTENT_TYPE 104  
# define CONTENT_LENGTH 105    
# define SCRIPT_FILENAME 106   
# define SCRIPT_NAME 107  
# define REQUEST_URI 108    
# define DOCUMENT_URI 109  
# define DOCUMENT_ROOT 110  
# define SERVER_PROTOCOL 111  
# define REMOTE_ADDR 112
# define REMOTE_PORT 113
# define SERVER_ADDR 114    
# define SERVER_PORT 115    
# define SERVER_NAME 116	 

struct s_method {
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
		void adding_param_map(int const& key, std::string const& value);

		void set_root(std::string const & root);
		void set_autoindex(bool const & autoindex);
		void set_return(int const & code, std::string const & path);

        bool get_method(int nb);
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
		std::map<int, std::string>  _fastcgi_param_map;
        std::vector<std::string> 	_fastcgi_pass;
		std::pair<int, std::string> _return;

};

#endif