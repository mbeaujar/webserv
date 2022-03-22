#include "Cgi.hpp"

Cgi::Cgi(std::string path, int port)
    : _pid(-1), _fd_in(-1), _fd_out(-1), _argv(NULL), _envp(NULL), _content(), _path_cgi(path), _path_file_in(),
      _path_file_out(), _body(), _port(port)
{
}

Cgi::Cgi(Cgi const &copy)
    : _pid(copy._pid), _fd_in(copy._fd_in), _fd_out(copy._fd_out), _argv(copy._argv), _envp(copy._envp),
      _content(copy._content), _path_cgi(copy._path_cgi), _path_file_in(copy._path_file_in),
      _path_file_out(copy._path_file_out), _body(copy._body), _port(copy._port)
{
}

Cgi::~Cgi()
{
    if (_argv)
        this->free_tab(_argv, FREE_LINEAR);
    if (_envp)
        this->free_tab(_envp, FREE_LINEAR);
    if (_fd_in != -1)
        close(_fd_in);
    if (_fd_out != -1)
        close(_fd_out);
}

Cgi &Cgi::operator=(Cgi const &copy)
{
    if (this != &copy)
    {
        _pid = copy._pid;
        _fd_in = copy._fd_in;
        _fd_out = copy._fd_out;
        _argv = copy._argv;
        _envp = copy._envp;
        _content = copy._content;
        _path_cgi = copy._path_cgi;
        _path_file_in = copy._path_file_in;
        _path_file_out = copy._path_file_out;
        _body = copy._body;
        _port = copy._port;
    }
    return *this;
}

std::string &Cgi::execute(Request &request, std::string method, int &client_socket, std::string &path_in,
                          std::string &content_type, std::string &stdin)
{
    std::string file;
    int status;
    bool ret;

    _path_file_in = path_in;
    file = ".cgi_" + to_string(client_socket);
    _fd_out = this->create_file(file);
    _fd_in = open(stdin.c_str(), O_RDWR);
    if (_fd_in == -1 || _fd_out == -1)
        return this->error(request, "can't create/open file");
    ret = this->create_argv();
    if (ret == EXIT_FAILURE)
        return this->error(request, "can't create argv");
    _query_string = request.get_query_string();
    ret = this->create_envp(method, content_type, request);
    if (ret == EXIT_FAILURE)
        return this->error(request, "can't create envp");
    _pid = fork();
    if (_pid == -1)
        return this->error(request, "can't create child process");
    if (_pid == 0)
    {
        dup2(_fd_in, 0);
        dup2(_fd_out, 1);
        execve(_argv[0], _argv, _envp);
        exit(EXIT_FAILURE);
    }
    waitpid(_pid, &status, 0);
    this->parse(request, client_socket);
    remove_file(file.c_str());
    return _body;
}

// ------------------------ PRIVATE --------------------------- //

char *Cgi::strdup(std::string const &s1)
{
    int i;
    char *s2;

    i = 0;
    while (s1[i])
        ++i;
    try
    {
        s2 = new char[i + 1];
    }
    catch (const std::exception &e)
    {
        std::cerr << "webserv: [warn]: class Cgi: Method strdup: " << e.what() << "\n";
        return NULL;
    }
    i = 0;
    while (s1[i])
    {
        s2[i] = s1[i];
        ++i;
    }
    s2[i] = '\0';
    return s2;
}

void Cgi::free_tab(char **tab, int len)
{
    if (tab)
    {
        if (len != FREE_LINEAR)
        {
            for (int i = 0; i < len; i++)
            {
                if (tab[i])
                    delete[] tab[i];
            }
        }
        else
        {
            for (int i = 0; tab[i]; i++)
                delete[] tab[i];
        }
        delete[] tab;
    }
}

int Cgi::create_argv(void)
{
    try
    {
        _argv = new char *[3];
    }
    catch (const std::exception &e)
    {
        std::cerr << "webserv: [warn]: class Cgi: Method create_argv: " << e.what() << '\n';
        _argv = NULL;
        return EXIT_FAILURE;
    }
    _argv[0] = this->strdup(_path_cgi.c_str());
    _argv[1] = this->strdup(_path_file_in.c_str());
    _argv[2] = NULL;
    if (_argv[0] == NULL || _argv[1] == NULL)
    {
        this->free_tab(_argv, 3);
        _argv = NULL;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

std::string Cgi::path_dir(std::string path)
{
    int i = path.length();
    while (i > 0 && path[i] != '/')
        i--;
    return path.substr(0, i);
}

int Cgi::create_envp(std::string &method, std::string &content_type, Request &request)
{
    std::string &header = request.get_header();
    size_t count_line_header = 0;
    size_t len_tab;

    for (size_t i = 0; header[i]; i++)
    {
        if (header[i] == '\n')
            count_line_header++;
    }
    len_tab = 18 + --count_line_header;
    try
    {
        _envp = new char *[len_tab];
    }
    catch (const std::exception &e)
    {
        std::cerr << "webserv: [warn]: class Cgi: Method create_envp: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    std::string cgi_dir = path_dir(_path_cgi);
    std::string file_dir = path_dir(_path_file_in);
    struct stat st;
    stat(_path_file_in.c_str(), &st);

    _envp[0] = this->strdup("CONTENT_TYPE=" + content_type);
    _envp[1] = this->strdup("CONTENT_LENGTH=" + to_string(st.st_size - 1));
    _envp[2] = this->strdup("SERVER_PROTOCOL=HTTP/1.1");
    _envp[3] = this->strdup("GATEWAY_INTERFACE=CGI/1.1");
    _envp[4] = this->strdup("REQUEST_METHOD=" + to_string(method));
    _envp[5] = this->strdup("PATH_INFO=" + request.get_path());
    _envp[6] = this->strdup("PATH_TRANSLATED=" + _path_file_in);
    _envp[7] = this->strdup("QUERY_STRING=" + _query_string);
    _envp[8] = this->strdup("REMOTE_ADDR=127.0.0.1");
    _envp[9] = this->strdup("REMOTE_USER="); // nothing after '=' is NULL
    _envp[10] = this->strdup("REMOTE_IDENT=");
    _envp[11] = this->strdup("REQUEST_URI=" + request.get_path());
    _envp[12] = this->strdup("REDIRECT_STATUS=200");
    _envp[13] = this->strdup("SERVER_NAME=webserv");
    _envp[14] = this->strdup("SERVER_SOFTWARE=webserv/1.0");
    _envp[15] = this->strdup("SERVER_PORT=" + to_string(_port));
    _envp[16] = this->strdup("SCRIPT_NAME=" + _path_file_in);
    size_t index = 0;
    size_t count = 0;
    for (; count < count_line_header; count++)
    {
        size_t endl = header.find('\n', index);
        std::string line = header.substr(index, endl - index);
        if (line.length() > 0 && line[line.length() - 1] == '\r')
            line.erase(--line.end());
        index = endl + 1;
        size_t equal = line.find(':', 0);
        std::string key = line.substr(0, equal);
        std::string value = line.substr(equal + 1, line.length() - (equal + 1));
        std::transform(key.begin(), key.end(), key.begin(), ::toupper);
        std::replace(key.begin(), key.end(), '-', '_');
        if (value[0] == ' ')
            value.erase(value.begin());
        _envp[17 + count] = this->strdup("HTTP_" + key + "=" + value);
    }
    _envp[17 + count] = NULL;
    len_tab = 17 + count;
    for (size_t i = 0; i < len_tab; i++)
    {
        if (_envp[i] == NULL)
        {
            this->free_tab(_envp, len_tab);
            _envp = NULL;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int Cgi::create_file(std::string name)
{
    int fd = -1;

    if (file_exist(name))
        fd = open(name.c_str(), O_RDWR);
    else
        fd = open(name.c_str(), O_CREAT | O_RDWR, S_IRWXO);
    if (fd == -1)
        std::cerr << "webserv: [warn]: class Cgi: Method create_file: can't create file: " << name << '\n';
    return fd;
}

std::string &Cgi::error(Request &request, std::string reason)
{
    request.set_error(std::make_pair(502, "Bad Gateway"));
    std::cerr << "webserv: [warn]: class Cgi: Method execution: " << reason << '\n';
    return request.get_query_string();
}

void Cgi::parse(Request &request, int &client_socket)
{
    std::string content = get_file_content(".cgi_" + to_string(client_socket));

    int len = this->header_size(content);
    std::string header = content.substr(0, len);
    _body = content.substr(len, content.length() - len);

    lower_file(header);
    this->parse_header(request, header);
}

int Cgi::header_size(std::string &response)
{
    int i = 0;
    while (response[i])
    {
        if (response.compare(i, 4, "\r\n\r\n") == 0)
            return i + 4;
        if (response.compare(i, 2, "\n\n") == 0)
            return i + 2;
        ++i;
    }
    return i;
}

void Cgi::parse_status(Request &request, std::string error)
{
    int code, i = 0;

    while (isspace(error[i]) && error[i] != '\n')
        ++i;
    code = recup_nb(error, i);
    while (isspace(error[i]) && error[i] != '\n')
        ++i;
    int skip = skip_line(error, i);
    request.set_error(std::make_pair(code, error.substr(i, skip - i)));
}

void Cgi::parse_header(Request &request, std::string &header)
{
    for (int i = 0; header[i]; i++)
    {
        if (header.compare(i, 8, "status: ") == 0)
        {
            i += 8;
            int skip = skip_line(header, i);
            parse_status(request, header.substr(i, skip - i));
            i = skip;
            if (header[i] == '\n')
                ++i;
        }
        if (header.compare(i, 14, "content-type: ") == 0)
        {
            i += 14;
            int skip = skip_line(header, i);
            request.set_content_type(header.substr(i, skip - i));
            i = skip;
            if (header[i] == '\n')
                ++i;
        }
        if (header.compare(i, 16, "content-length: ") == 0)
        {
            i += 16;
            int code = recup_nb(header, i);
            request.set_content_length(code);
            if (header[i] == '\n')
                ++i;
        }
    }
}

std::string Cgi::parse_cgi(Request &request, std::string response)
{
    int len = header_size(response);
    std::string header = response.substr(0, len);
    std::string body = response.substr(len, response.length() - len);

    lower_file(header);
    parse_header(request, header);
    return body;
}

std::string Cgi::cut_filename(std::string &path)
{
    int i = path.length();

    while (i > 0 && path[i] != '/')
        i--;
    return path.substr(i, path.length() - i);
}
