#include "Cgi.hpp"

Cgi::Cgi() :
	_pid(-1),
	_fd_in(-1),
	_fd_out(-1),
	_argv(NULL),
	_envp(NULL),
	_content(),
	_path_cgi(),
	_path_file_in(),
	_path_file_out(),
	_body() {}

Cgi::Cgi(Cgi const & copy) :
    _pid(copy._pid),
	_fd_in(copy._fd_in),
	_fd_out(copy._fd_out),
	_argv(copy._argv),
	_envp(copy._envp),
	_content(copy._content),
	_path_cgi(copy._path_cgi),
	_path_file_in(copy._path_file_in),
	_path_file_out(copy._path_file_out),
	_body(copy._body) {}

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


Cgi & Cgi::operator=(Cgi const & copy)
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
    }
    return *this;
}

std::string & Cgi::execute(Request & request, int method, int & client_socket, std::string & path_in)
{
	int status;
	bool ret;

	_fd_out = this->create_file(".post_" + to_string(client_socket));
	_fd_in = open(path_in.c_str(), O_RDONLY);
	if (_fd_in == -1 || _fd_out == -1)
		return this->error(request, "can't create/open file");
	ret = this->create_argv();
	if (ret == EXIT_FAILURE)
		return this->error(request, "can't create argv");
	_query_string = request.get_query_string();
	ret = this->create_envp(method);
	if (ret == EXIT_FAILURE)
		return this->error(request, "can't create envp");
	_pid = fork();
	if (_pid == -1)
		return this->error(request, "can't create child process");
	if (_pid == 0)
	{
		dup2(_fd_in, 0);
		dup2(_fd_out, 1);
		execve(_path_cgi.c_str(), _argv, _envp);
		exit(EXIT_FAILURE);
	}
	waitpid(_pid, &status, 0);
	this->parse(request, client_socket);
	return _body;
}

// ------------------------ PRIVATE --------------------------- //

char* Cgi::strdup(std::string const & s1)
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
	catch(const std::exception& e)
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
					delete [] tab[i];
			}
		}
		else
		{
			for (int i = 0; tab[i]; i++)
				delete [] tab[i];
		}
		delete [] tab;
	}
}


int Cgi::create_argv(void)
{
	try
	{
		_argv = new char*[3];
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: [warn]: class Cgi: Method create_argv: "<< e.what() << '\n';
		_argv = NULL;
		return EXIT_FAILURE;
	}
	_argv[0] = this->strdup(_path_cgi.c_str());
	_argv[1] = this->strdup(_path_file_in.c_str());
	_argv[2] = NULL;
	if (_argv[0] == NULL || _argv[2] == NULL)
	{
		this->free_tab(_argv, 3);
		_argv = NULL;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Cgi::create_envp(int & method)
{
	try
	{
		_envp = new char*[7];
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: [warn]: class Cgi: Method create_envp: "<< e.what() << '\n';
		return EXIT_FAILURE;
	}
	std::string cgi_dir = path_dir(_path_cgi);
	_envp[0] = this->strdup("REQUEST_METHOD=" + to_string(method));
	_envp[1] = this->strdup("SERVER_PROTOCOL=HTTP/1.1");
	_envp[2] = this->strdup("PATH_INFO=" + cgi_dir);
	_envp[3] = this->strdup("QUERY_STRING=" + _query_string);
	_envp[4] = this->strdup("REQUEST_URI=" + cgi_dir);
	_envp[5] = this->strdup("SCRIPT_FILENAME=" + _path_file_in);
	_envp[6] = NULL;
	for (int i = 0; i < 6; i++)
	{
		if (_envp[i] == NULL)
		{
			this->free_tab(_envp, 6);
			_envp = NULL;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int Cgi::create_file(std::string name)
{
	if (file_exist(name))
		remove_file(name.c_str());
	int fd = open(name.c_str(), O_CREAT | O_RDWR, S_IRWXO);
	if (fd == -1)
		std::cerr << "webserv: [warn]: class Cgi: Method create_file: can't create file: "<< name << '\n';
	return fd;
}

std::string & Cgi::error(Request & request, std::string reason)
{
	request.set_error(std::make_pair(502, "Bad Gateway"));
	std::cerr << "webserv: [warn]: class Cgi: Method execution: " << reason << '\n';
	return request.get_query_string();
}

void Cgi::parse(Request & request, int & client_socket)
{
	std::string content = get_file_content(".post_" + to_string(client_socket));

	int len = this->header_size(content);
	std::string header = content.substr(0, len);
	_body = content.substr(len, content.length() - len);

	lower_file(header);
	this->parse_header(request, header);
}


int Cgi::header_size(std::string & response)
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

int Cgi::skip_line(std::string line, int i)
{
	while (line[i] && line[i] != '\n')
		++i;
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

void Cgi::parse_header(Request &request, std::string header)
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


