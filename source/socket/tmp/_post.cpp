#include "../socket.hpp"

// response body ? : https://stackoverflow.com/questions/1226810/is-http-post-request-allowed-to-send-back-a-response-body

// RFC post : https://datatracker.ietf.org/doc/html/rfc7231#section-4.3.3

// multipart/form-data


bool path_upload(std::string path, std::string upload)
{
	if (path.length() < upload.length())
		return false;
	int i = 0;
	while (upload[i] && upload[i] == path[i])
		++i;
	if (upload[i])
		return false;
	return true;
}

int parse_post(Server const & server, Request & request, int client_socket)
{
	std::string tmp_file;
	std::string response;
	std::string path;
	Location location;
	int fd;

	tmp_file = ".post_" + to_string(client_socket);
	location = find_location(request, server, POST);
	if (ISERROR(request.get_error().first))
	{
		std::cerr << "webserv: [warn]: parse_post: can't find location" << std::endl;
		return EXIT_FAILURE;
	}
	// recup le path
	path = path_to_file(request, location, POST);
	if (ISERROR(request.get_error().first))
	{
		std::cerr << "webserv: [warn]: parse_post: can't find path" << std::endl;
		return EXIT_FAILURE;
	}

	// si le chemin existe pas
	if (file_exist(path) == false)
	{
		// on upload ??
		if (path_upload(path, location.get_upload()) == false)
		{
			// code erreur si on peux pas upload ??
			
			std::cerr << "webserv: [warn]: parse_post: path upload does not exist" << std::endl;
			return EXIT_FAILURE;
		}
		request.set_error(std::make_pair(201, "Created"));
	}

	if (is_directory(path))
	{
		if (path.length() > 1 && path[path.length() - 1] != '/')
			path.insert(path.end(), '/');
		std::vector<std::string> index = location.get_index();
		if (index.size() > 1)
			path += location.get_index()[0];
	}

	if ((fd = open(tmp_file.c_str(), O_CREAT | O_RDWR, S_IRWXO)) == -1)
	{
		std::cerr << "webserv: [warn]: parse_post: can't open tmp_file: " << tmp_file << std::endl;
		request.set_error(std::make_pair(500, "Internal Server Error"));
		return EXIT_FAILURE;
	}
	// std::cerr << "CGI" << std::endl;
	if (request.get_content_length() >= 0)
	{
		if (read_body(client_socket, location.get_max_body(), fd, request.get_content_length()))
		{
			request.set_error(std::make_pair(500, "Internal Server Error"));
			std::cerr << "webserv: [warn]: parse_post: can't read body" << std::endl;
			remove_file(tmp_file.c_str());
			close(fd);
			return EXIT_FAILURE;
		}
	}
	else
	{ // chunked request
		if (read_body_chunked(client_socket, location.get_max_body(), fd))
		{
			request.set_error(std::make_pair(500, "Internal Server Error"));
			std::cerr << "webserv: [warn]: parse_post: can't read body chunked" << std::endl;
			remove_file(tmp_file.c_str());
			close(fd);
			return EXIT_FAILURE;
		}
	}

	if (is_extension(path, location.get_cgi_ext()) == true)
	{
		if (file_exist(location.get_path_cgi()) == false)
		{
			request.set_error(std::make_pair(502, "Bad Gateway"));
			std::cerr << "webserv: [warn]: parse_post: file does not exist" << std::endl;
			remove_file(tmp_file.c_str());
			close(fd);
			return EXIT_FAILURE;
		}
		response = call_cgi(request, client_socket, tmp_file, "POST", location.get_path_cgi());
		if (response.empty() == true)
			request.set_error(std::make_pair(502, "Bad Gateway"));
		else
			response = parse_cgi(request, response);
	}
	else
	{
		response = get_file_content(tmp_file);
	}
	remove_file(tmp_file.c_str());
	close(fd);

	
	if (set_file_content(path, response) == EXIT_FAILURE)
	{
		std::cerr << "webserv: [warn]: parse_post: can't open: " << path << std::endl;
		request.set_error(std::make_pair(500, "Internal Server Error"));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int method_post(Server const & server, Request & request, int client_socket)
{
	try
	{
		parse_post(server, request, client_socket);
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: [warn]: method_post: " << e.what() << std::endl;
		request.set_error(std::make_pair(500, "Internal Server Error"));
	}
	send_response(request, "", client_socket, server);
	return EXIT_SUCCESS;
}
