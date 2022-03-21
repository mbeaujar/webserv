#include "Request.hpp"

Request::Request(int &client_socket)
    : _method(), _content_length(-1), _host(), _path(), _file(), _date(), _query_string(), _content_type(), _boundary(),
      _methods(), _cookie_username(), _cookie_color(), _new_client(true), _error(200, "OK"), _return(-1, ""), _accept()
{
    int len = 0;
    char *header = this->read_header(client_socket);

    if (header)
        len = strlen(header);
    // std::cerr << header << std::endl;
    if (header == NULL)
    {
        std::cerr << "webserv: [warn]: class Request: Constructor: can't read "
                     "header request"
                  << std::endl;
        this->set_error(std::make_pair(500, "Internal Server Error"));
    }
    else if (header && len == 0)
    {
        std::cerr << "webserv: [warn]: class Request: Constructor: empty header in "
                     "the request"
                  << std::endl;
        this->set_error(std::make_pair(400, "Bad Request"));
    }
    else if (len > MAX_LEN - 1)
    {
        std::cerr << "webserv: [warn]: class Request: Constructor: header in the "
                     "request too large"
                  << std::endl;
        this->set_error(std::make_pair(413, "Request Entity Too Large"));
    }
    else
        parse_header(header);
    delete[] header;
}

Request::Request(Request const &src)
    : _method(src._method), _content_length(src._content_length), _host(src._host), _path(src._path), _file(src._file),
      _date(src._date), _query_string(src._query_string), _content_type(src._content_type), _boundary(src._boundary),
      _methods(src._methods), _cookie_username(src._cookie_username), _cookie_color(src._cookie_color),
      _new_client(src._new_client), _error(src._error), _return(src._return), _accept(src._accept)
{
}

Request::~Request()
{
}

Request &Request::operator=(Request const &rhs)
{
    if (this != &rhs)
    {
        this->_method = rhs._method;
        this->_content_length = rhs._content_length;
        this->_host = rhs._host;
        this->_path = rhs._path;
        this->_file = rhs._file;
        this->_date = rhs._date;
        this->_query_string = rhs._query_string;
        this->_content_type = rhs._content_type;
        this->_boundary = rhs._boundary;
        this->_methods = rhs._methods;
        this->_error = rhs._error;
        this->_return = rhs._return;
        this->_cookie_username = rhs._cookie_username;
        this->_cookie_color = rhs._cookie_color;
        this->_new_client = rhs._new_client;
    }
    return *this;
}

void Request::set_file(std::string &file)
{
    if (is_directory(file))
    {
        if (file.length() > 1 && file[file.length() - 1] != '/')
            file.push_back('/');
    }
    _file = file;
}

void Request::set_method(int const &method)
{
    _method = method;
}
void Request::set_path(std::string const &path)
{
    _path = path;
}
void Request::set_date(std::string const &date)
{
    _date = date;
}
void Request::set_host(std::string const &host)
{
    _host = host;
}
void Request::set_content_length(int &len)
{
    _content_length = len;
}
void Request::set_boundary(std::string const &boundary)
{
    _boundary = boundary;
}
void Request::set_methods(struct s_method &met)
{
    _methods = met;
}
void Request::set_return(std::pair<int, std::string> const &r)
{
    _return = r;
}
void Request::set_content_type(std::string const &type)
{
    _content_type = type;
}
void Request::set_error(std::pair<int, std::string> const &error)
{
    _error = error;
}
void Request::set_accept(std::map<std::string, int> const &accept)
{
    _accept = accept;
}

void Request::set_query_string(std::string const &query_string)
{
    if (_query_string.length() != 0) // if already exist we concatenate
    {
        _query_string += "&";
        _query_string = query_string;
    }
    else
        _query_string = query_string;
}

void Request::generate_cookie_username(void)
{
    std::string tab[5] = {"John", "Kenny", "Mohammed", "Toufik"};
    srand(time(NULL));
    _cookie_username = tab[rand() % 4];
}

void Request::generate_cookie_color(void)
{
    std::string tab[3] = {"red", "green", "blue"};
    srand(time(NULL));
    _cookie_color = tab[rand() % 3];
}

int &Request::get_method()
{
    return _method;
}
int &Request::get_content_length()
{
    return _content_length;
}
std::string &Request::get_host()
{
    return _host;
}
std::string &Request::get_date()
{
    return _date;
}
std::string &Request::get_query_string()
{
    return _query_string;
}
std::string &Request::get_content_type()
{
    return _content_type;
}
std::string &Request::get_boundary()
{
    return _boundary;
}
std::string &Request::get_cookie_username()
{
    return _cookie_username;
}
std::string &Request::get_cookie_color()
{
    return _cookie_color;
}
std::string &Request::get_path()
{
    return _path;
}
struct s_method &Request::get_methods()
{
    return _methods;
}
std::string &Request::get_file()
{
    return _file;
}
std::pair<int, std::string> &Request::get_error()
{
    return _error;
}
std::map<std::string, int> &Request::get_accept()
{
    return _accept;
}
std::pair<int, std::string> &Request::get_return()
{
    return _return;
}
bool &Request::get_new_client()
{
    return _new_client;
}
std::string &Request::get_header()
{
    return _header;
}

bool Request::is_query(std::string &path)
{
    return path.find('?') != std::string::npos;
}

std::string Request::parse_query(std::string &path)
{
    size_t pos = path.find('?');
    _query_string = path.substr(pos + 1, path.length() - (pos + 1));
    return path.substr(0, pos);
}

void lower_file(std::string &request)
{
    int i = 0;
    while (request[i])
    {
        if (request[i] >= 'A' && request[i] <= 'Z')
            request[i] += 32;
        if (request[i] == ':' || request[i] == ' ')
        {
            while (request[i] && request[i] != '\n')
                ++i;
        }
        if (request[i])
            ++i;
    }
}

int Request::skip_the_word(std::string &file, int i)
{
    while (file[i] && !isspace(file[i]))
        ++i;
    return i;
}

std::string Request::recup_word(std::string &request, int &i)
{
    int tmp;
    std::string word;

    tmp = skip_the_word(request, i);
    word = request.substr(i, tmp - i);
    i = skip_space(request, tmp);
    return word;
}

int Request::get_first_line(std::string &request)
{
    int i = 0;
    std::string word;

    word = recup_word(request, i);
    if (word == "delete")
        this->set_method(DELETE);
    else if (word == "post")
        this->set_method(POST);
    else if (word == "get")
        this->set_method(GET);
    else
        this->set_method(OTHER);
    word = recup_word(request, i);
    if (word.length() > 1 && word[word.length() - 1] == '/')
        word.erase(--word.end());
    if (this->is_query(word) == true)
        word = this->parse_query(word);
    this->set_path(word);
    word = recup_word(request, i);
    if (word != "HTTP/1.1")
    {
        std::cerr << "webserv: [warn]: get_first_line: Bad HTTP version: " << word << std::endl;
        this->set_error(std::make_pair(505, "HTTP Version Not Supporteds"));
    }

    word = recup_word(request, i);
    if (word != "host:")
    {
        std::cerr << "webserv: [warn]: get_first_line: Request without host part" << std::endl;
        this->set_error(std::make_pair(400, "Bad request"));
    }
    else
    {
        word = recup_word(request, i);
        this->set_host(word);
    }
    return i;
}

char *Request::read_header(int client_socket)
{
    int bytes_read, msgsize;
    char *buffer;

    try
    {
        buffer = new char[MAX_LEN];
    }
    catch (std::exception &e)
    {
        std::cerr << "webserv: [warn]: read_header: " << e.what() << std::endl;
        return NULL;
    }
    FIX_BROKEN_PIPE;
    memset(buffer, 0, MAX_LEN);
    msgsize = 0;
    while ((bytes_read = recv(client_socket, &buffer[msgsize], 1, 0)) > 0)
    {
        msgsize += bytes_read;
        if (msgsize > MAX_LEN - 1)
            break;
        if (msgsize > 4 && buffer[msgsize - 1] == '\n' &&
            (strcmp(buffer + (msgsize - 1) - 1, "\n\n") == 0 || strcmp(buffer + (msgsize - 1) - 3, "\r\n\r\n") == 0))
            break;
    }
    if (msgsize <= MAX_LEN - 1)
        buffer[msgsize] = 0;
    return buffer;
}

void Request::parse_header(std::string request)
{
    int i = 0;
    int host = 0;
    int chunked = 0;

    _new_client = true;
    lower_file(request);
    i = get_first_line(request);
    _header = request.substr(i, request.length() - i); // (BIG COPY) only for cgi
    // std::cerr << "HEADER: \n" << request << std::endl;
    // std::cerr << "--------------------------------------------" << std::endl;
    while (request[i])
    {
        if (request.compare(i, 7, "accept:") == 0)
        {
            i += 8;
            std::string line = request.substr(i, skip_the_word(request, i) - i);
            this->parse_accept(line);
        }
        else if (request.compare(i, 7, "cookie:") == 0)
        {
            i += 8;
            std::string line = request.substr(i, (skip_line(request, i) - 1) - i);
            this->parse_cookie(line);
        }
        else if (request.compare(i, 15, "content-length:") == 0)
        {
            i += 16;
            int nb = recup_nb(request, i);
            this->set_content_length(nb);
        }
        else if (request.compare(i, 13, "content-type:") == 0)
        {
            // Exmple of boundary multipart/form-data; boundary=---------------------------735323031399963166993862150
            i += 14;
            size_t semicolon = request.find(';', i);
            this->set_content_type(request.substr(i, semicolon - i));
            if (get_content_type() == "multipart/form-data")
            {
                for (; request[semicolon] != '='; semicolon++)
                    ;
                this->set_boundary(
                    "--" + request.substr(semicolon + 1, (request.find('\n', semicolon) - 1) - (semicolon + 1)));
                // std::cout << _boundary << std::endl;
            }
        }
        else if (request.compare(i, 18, "transfer-encoding:") == 0)
        {
            i += 19;
            while (request[i] && request[i] != '\n')
            {
                std::string word = request.substr(i, skip_the_word(request, i) - i);
                if (word != "chunked")
                    this->set_error(std::make_pair(501, "Not Implemented"));
                chunked = 1;
                i = skip_the_word(request, i);
                if (request[i] != '\n')
                    ++i;
            }
        }
        else if (request.compare(i, 5, "date:") == 0)
        {
            i += 5;
            int tmp = i;
            i = skip_line(request, i);
            this->set_date(request.substr(tmp, i - tmp));
        }
        else
        {
            i = skip_line(request, i);
            if (request[i])
                ++i;
        }
    }
    if (_method == POST && _content_length == -1 && chunked == 0)
    {
        std::cerr << "webserv: [warn]: class Request: parse_header: request "
                     "without content-length and chunked"
                  << std::endl;
        this->set_error(std::make_pair(411, "Length Required"));
    }
    if (this->get_host().empty() || host > 1)
    {
        std::cerr << "webserv: [warn]: class Request: parse_header: none or too "
                     "many Host field"
                  << std::endl;
        this->set_error(std::make_pair(400, "Bad request"));
    }
    if (this->get_path().length() > 2048)
    {
        std::cerr << "webserv: [warn]: class Request: parse_header: path to long "
                     "(more than 2048 characters)"
                  << std::endl;
        this->set_error(std::make_pair(400, "Bad request"));
    }
}

void Request::print_request(void)
{
    std::cout << "----------- Request -----------" << std::endl;
    std::cout << " -> Host     : -> " << this->get_host() << std::endl;
    std::cout << " -> Type     : -> " << this->get_content_type() << std::endl;
    std::cout << " -> Path     : -> " << this->get_path() << std::endl;
    std::cout << " -> Method   : -> " << this->get_method() << std::endl;
    std::cout << " -> Length   : -> " << this->get_content_length() << std::endl;
    // std::cout << " -> Encoding : -> " << a.get_transfer_encoding() <<
    // std::endl;
    std::cout << " -> Error    : -> " << this->get_error().first << " -> " << this->get_error().second << std::endl;
    std::cout << " -> Return   : -> " << this->get_return().first << " -> " << this->get_return().second << std::endl;
    std::cout << " -> Query_string : -> " << this->get_query_string() << std::endl;
    std::cout << "-------------------------------" << std::endl;
}

void Request::parse_accept(std::string &content)
{
    size_t first = 0;
    size_t pos = 0;

    while (pos != std::string::npos)
    {
        pos = content.find(',', first);
        std::string new_accept = content.substr(first, pos - first);
        size_t l = new_accept.find(';', 0);
        if (l != std::string::npos)
            new_accept = new_accept.substr(0, l);
        if (new_accept.empty() == false)
            _accept.insert(std::make_pair(new_accept, 0));
        first = pos + 1;
    }
}

void Request::parse_cookie(std::string &content)
{
    size_t pos = 0;
    size_t first = 0;

    std::cout << "Cookie: " << content << std::endl;
    while (pos != std::string::npos)
    {
        pos = content.find(';', first);
        std::string cookie = content.substr(first, pos - first);
        size_t l = cookie.find('=', 0);
        size_t i = 0;
        while (cookie[i] && cookie[i] == ' ')
            i++;
        std::string key = cookie.substr(i, l - i);
        if (key == "username")
            _cookie_username = cookie.substr(l + 1, cookie.length() - (l + 1));
        else if (key == "color")
            _cookie_color = cookie.substr(l + 1, cookie.length() - (l + 1));

        first = pos + 1;
    }
    if (_cookie_username != "")
        _new_client = false;
    else
        _new_client = true;
}
