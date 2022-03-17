#include "Post.hpp"

Post::Post(Server &server, Request &request, int &client_socket, int &port)
    : AMethods(server, request, POST, client_socket, port), _file_fd(-1), _content_length(0), _ret(-1), _totalsize(0),
      _clientmax(0), _buffer(NULL), _file_name(".post_" + to_string(client_socket))
{
    try
    {
        _buffer = new char[BUFFERSIZE];
    }
    catch (const std::exception &e)
    {
        std::cerr << "webserv: [warn]: class Post: can't allocate buffer: " << e.what() << '\n';
        _buffer = NULL;
    }
}

Post::Post(Post const &src)
    : AMethods(src._server, src._request, POST, src._client_socket, src._port), _file_fd(src._file_fd),
      _content_length(src._content_length), _ret(src._ret), _totalsize(src._totalsize), _clientmax(src._clientmax),
      _buffer(src._buffer), _file_name(".post_" + to_string(_client_socket))
{
    try
    {
        _buffer = new char[BUFFERSIZE];
    }
    catch (const std::exception &e)
    {
        std::cerr << "webserv: [warn]: class Post: can't allocate buffer: " << e.what() << '\n';
        _buffer = NULL;
    }
}

Post::~Post()
{
    // if (file_exist(_file_name))
    //     remove_file(_file_name.c_str());
    if (_file_fd != -1)
        close(_file_fd);
    if (_buffer != NULL)
        delete[] _buffer;
    _buffer = NULL;
}

Post &Post::operator=(Post const &rhs)
{
    if (this != &rhs)
    {
        _method = rhs._method;
        _location = rhs._location;
        _path_file = rhs._path_file;
        _server = rhs._server;
        _request = rhs._request;
        _client_socket = rhs._client_socket;
        _body = rhs._body;
        _file_fd = rhs._file_fd;
        _content_length = rhs._content_length;
        _ret = rhs._ret;
        _totalsize = rhs._totalsize;
        _clientmax = rhs._clientmax;
        _buffer = rhs._buffer;
        _port = rhs._port;
    }
    return *this;
}

int Post::is_boundary(std::string &line)
{
    if (line == _request.get_boundary() + "\r\n")
        return BOUNDARY;
    std::string end = _request.get_boundary() + "--\r\n";
    if (line == end)
        return END_BOUNDARY;
    return NO_BOUNDARY;
}

std::string readline(int fd)
{
    std::string line;
    char c;
    int ret = 1;

    while (true)
    {
        ret = read(fd, &c, 1);
        if (ret == -1 || ret == 0 || c == '\n')
            break;
        if (c == '\r')
        {
            ret = read(fd, &c, 1);
            if (c == '\n')
                break;
        }
        line += c;
    }
    if (c == '\n' && ret != 0)
    {
        line += '\r';
        line += c;
    }

    return line;
}

void Post::read_boundary(void)
{
    int state = 0;
    bool first_boundary = false;
    std::string boundary_content;

    int fd_file = open(_file_name.c_str(), O_RDWR);
    while (true)
    {
        std::string line = readline(fd_file);
        state = is_boundary(line);
        if ((first_boundary == true && state == BOUNDARY) || state == END_BOUNDARY)
        {
            // std::cout << "ici" << std::endl;
            size_t blank_line = boundary_content.find("\r\n\r\n", 0);
            // std::cout << "boundary: \n" << boundary_content << std::endl;
            int fd = parse_boundary_header(boundary_content.substr(0, blank_line));
            if (blank_line != std::string::npos)
            {
                if (fd == -1)
                    _request.set_error(std::make_pair(500, "Internal Server Error"));
                else
                {
                    std::string body =
                        boundary_content.substr(blank_line + 4, boundary_content.length() - (blank_line + 6));
                    write(fd, body.c_str(), body.length());
                    boundary_content.clear();
                }
            }
            close(fd);
            if (state == END_BOUNDARY)
                break;
        }
        else
            first_boundary = true;
        if (state == NO_BOUNDARY)
            boundary_content += line;
        std::cout << "line: " << line << std::endl;
    }
    close(fd_file);
}

int Post::parse_boundary_header(std::string header)
{
    size_t pos;
    std::string name;
    std::string filename;
    std::string content_type;
    std::vector<std::string> boundary_header;

    while ((pos = header.find('\n')) != std::string::npos)
    {
        boundary_header.push_back(header.substr(0, pos));
        header.erase(0, pos + 1);
    }
    boundary_header.push_back(header);

    std::vector<std::string>::iterator it = boundary_header.begin(), ite = boundary_header.end();
    while (it != ite)
    {
        if (it->compare(0, 19, "Content-Disposition") == 0)
        {
            int i = 0;
            while ((*it)[i] != '\0')
            {
                if (it->compare(i, 4, "name") == 0)
                {
                    i += 6;
                    while ((*it)[i] != '\"')
                    {
                        name += (*it)[i];
                        i++;
                    }
                }
                else if (it->compare(i, 8, "filename") == 0)
                {
                    i += 10;
                    while ((*it)[i] != '\"')
                    {
                        filename += (*it)[i];
                        i++;
                    }
                }
                i++;
            }
        }
        if (it->compare(0, 12, "Content-Type") == 0)
        {
            int j = 13;
            while ((*it)[j] != '\0')
            {
                content_type += (*it)[j];
                j++;
            }
        }
        ++it;
    }
    std::cout << "filename:  " << filename << std::endl;
    std::cout << "name:  " << name << std::endl;
    std::string path_file = _location.get_upload() + "/" + ((filename != "") ? filename : name);
    return open(path_file.c_str(), O_CREAT | O_RDWR, S_IRWXU);
}

void Post::execute(std::map<std::string, std::string> &mime)
{
    static_cast<void>(mime);
    if (_buffer != NULL)
    {
        if (this->search_location() == EXIT_SUCCESS)
        {
            if (this->is_method_allowed() == true)
            {
                std::string content;

                // create file
                _file_fd = open(_file_name.c_str(), O_CREAT | O_RDWR, S_IRWXU);
                if (_file_fd == -1)
                {
                    _request.set_error(std::make_pair(502, "Bad Gateway"));
                    return;
                }

                // fd -> file in local
                if (_request.get_content_length() >= 0)
                    this->read_body();
                else
                    this->read_body_chunked();

                // query string
                bool is_app = APPEND;
                std::string path_cgi;

                if (_request.get_content_type() == "application/x-www-form-urlencoded")
                {
                    if ((path_cgi = _location.find_path_cgi(extension(_path_file))) != "")
                    {
                        Cgi a(path_cgi, _port);

                        content = get_file_content(_file_name);
                        _request.set_query_string(content);
                        content = a.execute(_request, "POST", _client_socket, _path_file, _request.get_content_type(),
                                            _path_file);
                        is_app = NOAPPEND;
                    }
                    else
                        return;
                }
                else if (_request.get_content_type() == "multipart/form-data")
                {
                    if (_location.get_upload() != "")
                    {
                        size_t i = 0;
                        std::string upload = _location.get_upload() + "/";
                        while (_path_file[i] && upload[i] && _path_file[i] == upload[i])
                            i++;
                        if (i == upload.size() - 1)
                            read_boundary();
                    }
                    else
                        _request.set_error(std::make_pair(401, "Unauthorized"));
                    return;
                }
                else
                {
                    if ((path_cgi = _location.find_path_cgi(extension(_path_file))) != "")
                    {
                        Cgi a(path_cgi, _port);
                        content = a.execute(_request, "POST", _client_socket, _file_name, _request.get_content_type(),
                                            _path_file);
                    }
                    else
                        content = get_file_content(_file_name);
                }

                if (ISERROR(_request.get_error().first))
                    return;

                this->set_file_content(_path_file, content, is_app);
            }
        }
    }
}

// ---------------------- PRIVATE ------------------------- //

int Post::read_buffer(int size)
{
    // std::cout << "SIZE: " << size << std::endl;
    _ret = recv(_client_socket, _buffer, size, 0);
    if (_ret == -1)
    {
        std::cout << "ERRNO: " << strerror(errno) << std::endl;
        std::cerr << "werbserv: [warn]: read_buffer: can't read fd: " << _client_socket << " with buffer of " << size
                  << std::endl;
    }
    return _ret;
}

int Post::read_endline(void)
{
    char c;

    _ret = recv(_client_socket, &c, 1, 0);
    if (_ret == -1)
    {
        std::cerr << "werbserv: [warn]: read_endline: recv can't read\n";
        return EXIT_FAILURE;
    }
    if (c == '\r')
    {
        _ret = recv(_client_socket, &c, 1, 0);
        if (_ret == -1)
        {
            std::cerr << "werbserv: [warn]: read_endline: recv can't read after '\\r'\n";
            return EXIT_FAILURE;
        }
        if (c != '\n')
        {
            std::cerr << "werbserv: [warn]: read_endline: recv can't read after '\\r'\n";
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int Post::read_body_child(int msgsize)
{
    if (msgsize == 0) // code d'erreur -> return 1 -- a test
        return 1;
    if (msgsize == -1)
        msgsize = 0;
    _buffer[msgsize] = 0;
    // std::cerr << _buffer << std::endl;
    if (msgsize > 0)
        write(_file_fd, _buffer, msgsize);

    return 0;
}

int Post::read_chunk_child(int msgsize)
{
    if (msgsize == 0) // a test
        return 1;
    if (msgsize == -1)
        msgsize = 0;
    _buffer[msgsize] = 0;
    if (msgsize > 0)
        write(_file_fd, _buffer, msgsize);
    return 0;
}

void Post::read_for(int (Post::*f)(int))
{
    int msgsize;

    std::cout << "LEN: " << _content_length << std::endl;

    for (_totalsize = 0; _totalsize < _content_length; _totalsize += msgsize)
    {
        msgsize = 0;
        memset(_buffer, 0, BUFFERSIZE);
        if (_totalsize + BUFFERSIZE < _content_length)
            msgsize = read_buffer(BUFFERSIZE);
        else
            msgsize = read_buffer(_content_length - _totalsize);
        if ((this->*f)(msgsize) == 1)
            break;
        if (msgsize < 0)
            msgsize = 0;
    }
}

void Post::read_body(void)
{
    _content_length = _request.get_content_length();
    if (_clientmax != 0 && _content_length > _clientmax)
        _content_length = _clientmax;
    this->read_for(&Post::read_body_child);
    if (_totalsize != _content_length)
    {
        std::cerr << "read " << _totalsize << " / " << _content_length << std::endl;
        std::cerr << "webserv: [warn]: class Post: read_body: read not enough\n";
    }
}

int Post::read_chunked_length(void)
{
    char buff;
    int nb = 0;
    _ret = 1;

    while (_ret)
    {
        _ret = recv(_client_socket, &buff, 1, 0);
        if (_ret <= 0 || buff == '\n' || buff == '\r')
        {
            if (buff == '\r')
                read_endline();
            return nb;
        }
        if ((buff >= '0' && buff <= '9') || (buff >= 'a' && buff <= 'f'))
        {
            nb *= 16;
            if (buff < 58)
                nb += (buff - 48);
            else
                nb += (buff - 87);
        }
    }
    return nb;
}

void Post::read_body_chunked(void)
{
    _clientmax = _location.get_max_body();

    while (true)
    {
        _content_length = this->read_chunked_length();
        if (_content_length == 0)
        {
            this->read_endline();
            break;
        }
        if (_location.get_max_body() == 0)
            _clientmax = _totalsize + _content_length;
        if (_totalsize + _content_length > _clientmax)
            _content_length = _clientmax - _totalsize;
        this->read_for(&Post::read_chunk_child);
        this->read_endline();
        if (_location.get_max_body() != 0 && _totalsize + _content_length > _location.get_max_body())
            break;
    }
}