#include "AMethods.hpp"

AMethods::AMethods() :
    _method(),
    _location(),
    _path_file() {}

AMethods::AMethods(AMethods const & src) :
    _method(src._method),
    _location(src._location),
    _path_file(src._path_file) {}

AMethods::~AMethods() {}

AMethods & AMethods::operator=(AMethods const & rhs) {
    if (this != &rhs)
    {
        _method = rhs._method;
        _location = rhs._location;
        _path_file = rhs._path_file;
    }
    return *this;
}

void			AMethods::set_method(int const & method) { _method = method; }
void			AMethods::set_location(Location const & location) { _location = location; }
void			AMethods::set_path_file(std::string const & path_file) { _path_file = path_file; }

int &			AMethods::get_method() { return _method; }
Location &		AMethods::get_location() { return _location; }
std::string &	AMethods::get_path_file() { return _path_file; }
