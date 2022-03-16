#include "Location.hpp"

/**
 * @brief Construct a new Location::Location object
 *
 */
Location::Location()
    : _autoindex(false), _max_body(0), _root("/var/www/html"), _index(), _method(), _upload(), _cgi(), _return(-1, "")
{
}

/**
 * @brief Destroy the Location::Location object
 *
 */
Location::~Location()
{
}

/**
 * @brief Construct a new Location::Location object
 *
 * @fastcgi_param copy
 */
Location::Location(Location const &copy)
    : _autoindex(copy._autoindex), _max_body(copy._max_body), _root(copy._root), _index(copy._index),
      _method(copy._method), _upload(copy._upload), _cgi(copy._cgi), _return(copy._return)
{
}

/**
 * @brief
 *
 * @fastcgi_param copy
 * @return Location&
 */
Location &Location::operator=(Location const &copy)
{
    if (this == &copy)
        return *this;
    _autoindex = copy._autoindex;
    _max_body = copy._max_body;
    _root = copy._root;
    _index = copy._index;
    _method = copy._method;
    _upload = copy._upload;
    _cgi = copy._cgi;
    _return = copy._return;
    return *this;
}

bool Location::get_method(int nb)
{
    if (nb == DELETE)
    {
        if (_method.m_delete == true)
            return true;
        return false;
    }
    else if (nb == GET)
    {
        if (_method.m_get == true)
            return true;
        return false;
    }
    else if (nb == POST)
    {
        if (_method.m_post == true)
            return true;
        return false;
    }
    return false;
}

s_method &Location::get_methods()
{
    return _method;
}

void Location::adding_method(int nb)
{

    if (nb == DELETE)
        _method.m_delete = true;
    else if (nb == GET)
        _method.m_get = true;
    else
        _method.m_post = true;
}

void Location::adding_index(std::string const &index)
{
    _index.push_back(index);
}

void Location::set_root(std::string const &root)
{
    _root = root;
}

void Location::set_autoindex(bool const &autoindex)
{
    _autoindex = autoindex;
}

void Location::set_return(int const &code, std::string const &path)
{
    if (_return.first == -1)
    {
        _return.first = code;
        _return.second = path;
    }
}

void Location::set_max_body(int const &max_body)
{
    _max_body = max_body;
}
void Location::set_upload(std::string const &upload)
{
    _upload = upload;
}

void Location::set_cgi(std::string const &ext, std::string const &path)
{
    if (_cgi.find(ext) != _cgi.end())
        throw std::invalid_argument("invalid argument in \"cgi\" directive, doublon extension");
    _cgi.insert(std::make_pair(ext, path));
}

int &Location::get_max_body()
{
    return _max_body;
}
bool &Location::get_autoindex()
{
    return _autoindex;
}
std::string &Location::get_upload()
{
    return _upload;
}
std::string &Location::get_root()
{
    return _root;
}
std::vector<std::string> &Location::get_index()
{
    return _index;
}
std::pair<int, std::string> &Location::get_return()
{
    return _return;
}
std::map<std::string, std::string> &Location::get_cgi()
{
    return _cgi;
}

std::string Location::find_path_cgi(std::string const &ext)
{
    std::map<std::string, std::string>::iterator search;
    if ((search = _cgi.find(ext)) != _cgi.end())
        return search->second;
    return "";
}

bool Location::find_index(std::string const &index)
{
    std::vector<std::string>::const_iterator it = _index.begin(), ite = _index.end();

    while (it != ite)
    {
        if (*it == index)
            return true;
        ++it;
    }
    return false;
}
