#include "Response.hpp"

Response::Response(int &client_socket, Request &request, Server &server, int &port)
    : _port(port), _client_socket(client_socket), _request(request), _server(server), _response(), _method(NULL),
      _content_length(), _mime(), _error()
{
    this->init_type_mimes();
    this->init_error();
}

Response::Response(Response const &copy)
    : _port(copy._port), _client_socket(copy._client_socket), _request(copy._request), _server(copy._server),
      _response(copy._response), _method(copy._method), _content_length(copy._content_length), _mime(copy._mime),
      _error(copy._error)
{
}

Response::~Response()
{
    if (_method != NULL)
        delete _method;
}

Response &Response::operator=(Response const &copy)
{
    if (this != &copy)
    {
        _port = copy._port;
        _client_socket = copy._client_socket;
        _request = copy._request;
        _server = copy._server;
        _response = copy._response;
        _method = copy._method;
        _content_length = copy._content_length;
        _mime = copy._mime;
        _error = copy._error;
    }
    return *this;
}

void Response::execute(void)
{
    try
    {
        std::string body;

        if (ISERROR(_request.get_error().first) == false)
        {
            this->create_method(_request.get_method());
            if (_method != NULL)
            {
                _method->execute(_mime);
                if (_request.get_method() == GET)
                {
                    body = _method->get_body();
                    this->check_content_type(_method->get_path());
                }
            }
        }
        if ((ISERROR(_request.get_error().first) == true || ISREDIRECT(_request.get_return().first) == true) &&
            _request.get_method() == GET)
        {
            body = this->error_html();
            _request.set_content_type(_mime[".html"]);
        }
        _content_length = body.length();
        this->create_header();
        _response.append(body);
    }
    catch (const std::exception &e)
    {
        std::cerr << "webserv: [warn]: class Response: execute: " << e.what() << '\n';
        _request.set_error(std::make_pair(500, "Internal Server Error"));
    }
    this->send_response();
}

// --------------------- PRIVATE ----------------- //

void Response::send_response(void)
{
    // std::cerr << _response << std::endl;
    if (send(_client_socket, _response.c_str(), _response.length(), 0) == -1)
        std::cerr << "webserv: [warn]: class Response: send_response: can't write to client"
                  << "\n";
    if (shutdown(_client_socket, SHUT_RDWR) == -1)
        std::cerr << "webserv: [warn]: class Response: send_response: shutdown can't shut client and socket"
                  << "\n";
    close(_client_socket);
}

std::string Response::error_html(void)
{
    std::string status;
    std::string path;
    std::pair<int, std::string> error = _request.get_error();
    std::pair<int, std::string> redirect = _request.get_return();

    if (ISREDIRECT(redirect.first) == true)
    {
        if (redirect.first >= 300 && redirect.first <= 307 && redirect.first != 306)
        {
            std::map<int, std::string>::iterator search = _error.find(redirect.first);
            _request.set_error(std::make_pair(search->first, search->second));
            status = to_string(redirect.first) + " " + search->second;
        }
        else
        {
            _request.set_error(std::make_pair(redirect.first, ""));
            return redirect.second;
        }
    }
    else if (ISERROR(error.first) == true)
    {
        status = to_string(error.first) + " " + error.second;
        std::string path = _server.find_error(error.first);
        if (path.empty() == false)
        {
            if (file_exist(path))
                return get_file_content(path);
        }
    }
    path += "<html>\r\n";
    path += "<head><title>" + status + "</title></head>\r\n";
    path += "<body bgcolor=\"white\">\r\n";
    path += "<center><h1>" + status + "</h1></center>\r\n";
    path += "<hr><center>webserv/1.0 (Ubuntu)</center>\r\n";
    path += "</body>\r\n";
    path += "</html>\r\n";
    return path;
}

void Response::create_method(int &method)
{
    try
    {
        if (method == POST)
            _method = new Post(_server, _request, _client_socket, _port);
        else if (method == GET)
            _method = new Get(_server, _request, _client_socket, _port);
        else if (method == DELETE)
            _method = new Delete(_server, _request, _client_socket, _port);
    }
    catch (const std::exception &e)
    {
        std::cerr << "webserv: [warn]: class Response: create_method: " << e.what() << '\n';
        _method = NULL;
    }
}

std::string Response::allow_method(void)
{
    std::string method;
    s_method m = _request.get_methods();

    if (m.m_get == true)
        method += " GET";
    if (m.m_delete == true)
    {
        if (method != "")
            method += ",";
        method += " DELETE";
    }
    if (m.m_post == true)
    {
        if (method != "")
            method += ",";
        method += " POST";
    }
    return method;
}

// Tue, 07 Dec 2021 14:11:18 GMT
//  Wed, 21 Oct 2015 07:29:00 GMT;

std::string get_update_date(std::string date)
{
    size_t begin = date.find(':', 0);
    size_t end = date.find(':', begin + 1);

    std::string before = date.substr(0, begin);
    std::string min = date.substr(begin + 1, end - begin);
    std::string after = date.substr(end + 1, date.length() - end);
    size_t nb = atoi(min.c_str());
    nb++;
    std::string new_nb = to_string(nb);
    if (nb < 10)
        new_nb.insert(new_nb.begin(), '0');
    return before + ":" + new_nb + ":" + after;
}

std::string Response::get_hour_date(void)
{
    char buf[1000];
    time_t now = time(0);
    tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S GMT", &tm);
    std::string date(buf);
    return date;
}

std::string get_last_modified(std::string &path)
{
    tm *tm;
    char buf[1000];
    struct stat stats;

    memset(&stats, 0, sizeof(stats));
    stat(path.c_str(), &stats);
    tm = gmtime(&stats.st_mtime);
    strftime(buf, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
    std::string date(buf);
    return date;
}

void Response::create_header(void)
{
    std::pair<int, std::string> &error = _request.get_error();
    std::pair<int, std::string> &redirect = _request.get_return();

    _response = "HTTP/1.1 " + to_string(error.first) + " " + error.second + "\r\n";
    _response += "Server: webserv/1.0.0 (Ubuntu)\r\n";
    _response += "Date: " + this->get_hour_date() + "\r\n";
    _response += "Connection: close\r\n";
    if (_content_length > 0)
    {
        if (_request.get_content_type().empty() == false)
            _response += "Content-Type: " + _request.get_content_type() + "\r\n";
    }
    if (_request.get_method() == GET)
        _response += "Content-length: " + to_string(_content_length) + "\r\n";
    if (ISERROR(error.first) == false && IS_RANGE_REDIRECT(redirect.first) == false &&
        file_exist(_request.get_file()) == true)
        _response += "Last-Modified: " + get_last_modified(_request.get_file()) + "\r\n";
    if (error.first == 405)
        _response += "Allow: " + this->allow_method() + "\r\n";
    if (IS_RANGE_REDIRECT(redirect.first) || error.first == 201)
    {
        if (IS_RANGE_REDIRECT(redirect.first))
            _response += "Location: " + redirect.second + "\r\n";
        else
            _response += "Location: " + _request.get_path() + "\r\n";
    }
#if BONUS
    if (_request.get_new_client() == true)
    {
        _request.generate_cookie_username();
        _request.generate_cookie_color();
        std::cout << "Welcome " + _request.get_cookie_username() + " your color is " + _request.get_cookie_color()
                  << std::endl;
        _response += "Set-Cookie: username=" + _request.get_cookie_username() + "\r\n";
        _response += "Set-Cookie: color=" + _request.get_cookie_color() +
                     "; Expires=" + get_update_date(this->get_hour_date()) + ";\r\n";
    }
#endif
    _response += "\r\n"; // blank line
}

// 406 Not Acceptable

bool Response::set_content_type(std::string content_type, std::map<std::string, int> &accept)
{
    if (accept.size() > 0 && accept.find(content_type) == accept.end() && accept.find("*/*") == accept.end())
        return false;
    _request.set_content_type(content_type);
    return true;
}

void Response::check_content_type(std::string &path_file)
{
    std::string ext = extension(path_file);
    std::map<std::string, std::string>::iterator search;
    std::map<std::string, int> accept = _request.get_accept();

    if (ISERROR(_request.get_error().first) == false)
    {
        if (_request.get_content_type() == "")
        {
            if (ext != "" && (search = _mime.find(ext)) != _mime.end()) // exist in _mime
            {
                if (ext == ".3gp" || ext == ".3g2")
                {
                    if (this->set_content_type(search->second, accept) == false)
                    {
                        std::string special_case;
                        if (ext == ".3gp")
                            special_case = "audio/3gpp";
                        else
                            special_case = "audio/3gpp2";
                        if (this->set_content_type(search->second, accept) == false)
                            _request.set_error(std::make_pair(406, "Not Acceptable"));
                    }
                }
                else
                {
                    if (this->set_content_type(search->second, accept) == false)
                        _request.set_error(std::make_pair(406, "Not Acceptable"));
                }
            }
            else
                this->set_content_type(_mime["default"], accept);
        }
        else
        {
            std::string content_type = _request.get_content_type();
            size_t pos = 0;
            while (content_type[pos] && isspace(content_type[pos]) == false && content_type[pos] != ';')
                pos++;
            content_type = content_type.substr(0, pos);
            if (accept.size() > 0 && accept.find(content_type) == accept.end())
                _request.set_error(std::make_pair(406, "Not Acceptable"));
        }
    }
}

void Response::init_type_mimes(void)
{
    if (_mime.size() != 0)
        return;
    _mime["default"] = "text/html";
    _mime[".aac"] = "audio/aac";
    _mime[".abw"] = "application/x-abiword";
    _mime[".arc"] = "application/octet-stream";
    _mime[".avi"] = "video/x-msvideo";
    _mime[".azw"] = "application/vnd.amazon.ebook";
    _mime[".bin"] = "application/octet-stream";
    _mime[".bmp"] = "image/bmp";
    _mime[".bz"] = "application/x-bzip";
    _mime[".bz2"] = "application/x-bzip2";
    _mime[".csh"] = "application/x-csh";
    _mime[".css"] = "text/css";
    _mime[".csv"] = "text/csv";
    _mime[".doc"] = "application/msword";
    _mime[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    _mime[".eot"] = "application/vnd.ms-fontobject";
    _mime[".epub"] = "application/epub+zip";
    _mime[".gif"] = "image/gif";
    _mime[".htm"] = "text/html";
    _mime[".html"] = "text/html";
    _mime[".ico"] = "image/x-icon";
    _mime[".ics"] = "text/calendar";
    _mime[".jar"] = "application/java-archive";
    _mime[".jpeg"] = "image/jpeg";
    _mime[".jpg"] = "image/jpeg";
    _mime[".js"] = "application/javascript";
    _mime[".json"] = "application/json";
    _mime[".mid"] = "audio/midi";
    _mime[".midi"] = "audio/midi";
    _mime[".mpeg"] = "video/mpeg";
    _mime[".mpkg"] = "application/vnd.apple.installer+xml";
    _mime[".odp"] = "application/vnd.oasis.opendocument.presentation";
    _mime[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    _mime[".odt"] = "application/vnd.oasis.opendocument.text";
    _mime[".oga"] = "audio/ogg";
    _mime[".ogv"] = "video/ogg";
    _mime[".ogx"] = "application/ogg";
    _mime[".otf"] = "font/otf";
    _mime[".png"] = "image/png";
    _mime[".pdf"] = "application/pdf";
    _mime[".ppt"] = "application/vnd.ms-powerpoint";
    _mime[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    _mime[".rar"] = "application/x-rar-compressed";
    _mime[".rtf"] = "application/rtf";
    _mime[".sh"] = "application/x-sh";
    _mime[".svg"] = "image/svg+xml";
    _mime[".swf"] = "application/x-shockwave-flash";
    _mime[".tar"] = "application/x-tar";
    _mime[".tif"] = "image/tiff";
    _mime[".tiff"] = "image/tiff";
    _mime[".ts"] = "application/typescript";
    _mime[".ttf"] = "font/ttf";
    _mime[".vsd"] = "application/vnd.visio";
    _mime[".wav"] = "audio/x-wav";
    _mime[".weba"] = "audio/webm";
    _mime[".webm"] = "video/webm";
    _mime[".webp"] = "image/webp";
    _mime[".woff"] = "font/woff";
    _mime[".woff2"] = "font/woff2";
    _mime[".xhtml"] = "application/xhtml+xml";
    _mime[".xls"] = "application/vnd.ms-excel";
    _mime[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    _mime[".xml"] = "application/xml";
    _mime[".xul"] = "application/vnd.mozilla.xul+xml";
    _mime[".zip"] = "application/zip";
    _mime[".3gp"] = "video/3gpp";  // attention audio/3gpp si il accepte l'audio
    _mime[".3g2"] = "video/3gpp2"; // same audio/3gpp2
    _mime[".7z"] = "application/x-7z-compressed";
}

void Response::init_error(void)
{
    if (_error.size() > 0)
        return;
    _error[100] = "Continue";
    _error[101] = "Switching Protocols";
    _error[200] = "OK";
    _error[201] = "Created";
    _error[202] = "Accepted";
    _error[203] = "Non-Authoritative Information";
    _error[204] = "No Content";
    _error[205] = "Reset Content";
    _error[206] = "Partial Content";
    _error[300] = "Multiple Choices";
    _error[301] = "Moved Permanently";
    _error[302] = "Found";
    _error[303] = "See Other";
    _error[304] = "Not Modified";
    _error[305] = "Use Proxy";
    _error[307] = "Temporary Redirect";
    _error[400] = "Bad Request";
    _error[401] = "Unauthorized";
    _error[402] = "Payment Required";
    _error[403] = "Forbidden";
    _error[404] = "Not Found";
    _error[405] = "Method Not Allowed";
    _error[406] = "Not Acceptable";
    _error[407] = "Proxy Authentication Required";
    _error[408] = "Request Timeout";
    _error[409] = "Conflict";
    _error[410] = "Gone";
    _error[411] = "Length Required";
    _error[412] = "Precondition Failed";
    _error[413] = "Request Entity Too Large";
    _error[414] = "Request-URI Too Long";
    _error[415] = "Unsupported Media Type";
    _error[416] = "Requested Range Not Satisfiable";
    _error[417] = "Expectation Failed";
    _error[500] = "Internal Server Error";
    _error[501] = "Not Implemented";
    _error[502] = "Bad Gateway";
    _error[503] = "Service Unavailable";
    _error[504] = "Gateway Timeout";
    _error[505] = "HTTP Version Not Supported";
}