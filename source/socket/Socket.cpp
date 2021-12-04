#include "Socket.hpp"

bool g_exit;

Socket::Socket() :
	_max_fd(0),
	_current_sockets(),
	_current_clients(),
	_ready_sockets(),
	_ready_clients(),
	_threads_id(),
	_sockets(),
	_clients()
{
	FD_ZERO(&_current_sockets);
	FD_ZERO(&_current_clients);
	g_exit = false;
}

Socket::Socket(Socket const & copy) :
	_max_fd(copy._max_fd),
	_current_sockets(copy._current_sockets),
	_current_clients(copy._current_clients),
	_ready_sockets(copy._ready_sockets),
	_ready_clients(copy._ready_clients),
	_threads_id(copy._threads_id),
	_sockets(copy._sockets),
	_clients(copy._clients) {}


Socket::~Socket()
{
	std::vector<pthread_t>::iterator it = _threads_id.begin(), ite = _threads_id.end();

	while (it != ite)
	{
		pthread_join(*it, NULL); // block if there is a thread with a infinite loop
		++it;
	}
	this->release_fds(_clients);
	this->release_fds(_sockets);
}

Socket & Socket::operator=(Socket const & copy)
{
	if (this != &copy)
	{
		_max_fd = copy._max_fd;
		_current_sockets = copy._current_sockets;
		_current_clients = copy._current_clients;
		_ready_sockets = copy._ready_sockets;
		_ready_clients = copy._ready_clients;
		_threads_id = copy._threads_id;
		_sockets = copy._sockets;
		_clients = copy._clients;
	}
	return *this;
}

void Socket::execute(std::vector<Server> &servers)
{
	if (this->config_sockets(servers) == EXIT_FAILURE)
		return;
	std::map<int, Server>::iterator search, ite = _sockets.end();

	signal(SIGINT, signalHandler);
	signal(SIGPIPE, signalPipeHandler);
	while (true)
	{
		_ready_sockets = _current_sockets;
		_ready_clients = _current_clients;
		int fds = 0;
		if ((fds = select(_max_fd + 1, &_ready_sockets, &_ready_clients, NULL, NULL)) < 0)
		{
			if (g_exit == false)
				std::cerr << "webserv: [warn]: handle_socket: Failed to select: " << strerror(errno) << std::endl;
		}
		if (fds > 0 && g_exit == false)
		{
			try
				{
				for (int i = 0; i < _max_fd + 1; i++)
				{
					if (FD_ISSET(i, &_ready_sockets))
					{
						search = _sockets.find(i);
						if (search != ite)
						{
							int client_socket = this->accept_connection(i);
							if (client_socket != -1)
							{
								fcntl(client_socket, F_SETFL, O_NONBLOCK);
								if (client_socket > _max_fd)
									_max_fd = client_socket;
								_clients.insert(std::make_pair(client_socket, &search->second));
								FD_SET(client_socket, &_current_sockets);
								FD_SET(client_socket, &_current_clients);
							}
						}
						else if (FD_ISSET(i, &_ready_clients))
						{
							std::map<int, Server *>::iterator find = _clients.find(i);
							if (find != _clients.end())
							{
								this->create_thread(i, *find->second);
								_clients.erase(i);
								FD_CLR(i, &_current_sockets);
								FD_CLR(i, &_current_clients);
							}
						}
					}
				}
			}
			catch (std::exception &e)
			{
				std::cerr << "webserv: [warn]: class Socket: execute: " << e.what() << std::endl;
			}
		}
		if (g_exit == true)
			break;
	}
}

// ------------------------------ PRIVATE ----------------------------------- //

int Socket::config_sockets(std::vector<Server> &servers)
{
	std::vector<Server>::iterator it = servers.begin(), ite = servers.end();

	while (it != ite)
	{
		std::vector<Port> lst = it->get_port();
		std::vector<Port>::iterator begin = lst.begin(), end = lst.end();
		while (begin != end)
		{
			int server_socket;
			if (begin->ipv4)
				server_socket = this->create_socket_ipv4(begin->port, BACKLOG);
			else
				server_socket = this->create_socket_ipv6(begin->port, BACKLOG);
			begin->fd = server_socket;
			// it->set_current_port(begin->port); // ???????????????????????????????????????????????????????????
			if (server_socket == -1)
			{
				std::cerr << "webserv: [emerg]: class Socket: config_sockets: can't create socket" << std::endl;
				return EXIT_FAILURE;
			}
			if (server_socket > _max_fd)
				_max_fd = server_socket;
			FD_SET(server_socket, &_current_sockets);
			_sockets.insert(std::make_pair(server_socket, *it));
			++begin;
		}
		++it;
	}
	return EXIT_SUCCESS;
}

int Socket::create_socket_ipv4(int & port, int backlog)
{
	SA_IN server_addr;
	int server_socket;

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return -1;
	}
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
	#ifndef __APPLE__
		int opt = 1;
		socklen_t optlen = sizeof(opt);
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, optlen) == -1)
		{
			std::cerr << "webserv: [emerg]: class Socket: create_socket_ipv4: Failed to set socket options" << std::endl;
			close(server_socket);
			return -1;
		}
	#endif
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(server_socket, reinterpret_cast<SA*>(&server_addr), sizeof(server_addr)) == -1)
	{
		std::cerr << "webserv: [emerg]: class Socket: create_socket_ipv4: Failed to bind the socket" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		close(server_socket);
		return -1;
	}
	if (listen(server_socket, backlog) == -1)
	{
		std::cerr << "webserv: [emerg]: class Socket: create_socket_ipv4: Failed to listen" << std::endl;
		close(server_socket);
		return -1;
	}
	return server_socket;
}

int Socket::create_socket_ipv6(int & port, int backlog)
{
	SA_IN6 server_addr;
	int server_socket;

	if ((server_socket = socket(AF_INET6, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return -1;
	}
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
	#ifndef __APPLE__
		int opt = 1;
		socklen_t optlen = sizeof(opt);
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, optlen) == -1)
		{
			std::cerr << "webserv: [emerg]: class Socket: create_socket_ipv6: Failed to set socket options" << std::endl;
			close(server_socket);
			return -1;
		}
	#endif
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(port);
	server_addr.sin6_addr = in6addr_any; // <- 0.0.0.0
	// inet_pton(AF_INET6, "::1", &server_addr.sin6_addr); <- 127.0.0.1
	if (bind(server_socket, reinterpret_cast<SA*>(&server_addr), sizeof(server_addr)) == -1)
	{
		std::cerr << "webserv: [emerg]: class Socket: create_socket_ipv6: Failed to bind the socket" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		close(server_socket);
		return -1;
	}
	if (listen(server_socket, backlog) == -1)
	{
		std::cerr << "webserv: [emerg]: class Socket: create_socket_ipv6: Failed to listen" << std::endl;
		close(server_socket);
		return -1;
	}
	return server_socket;
}

int Socket::accept_connection(int server_socket)
{
	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	if ((client_socket = accept(server_socket, reinterpret_cast<SA*>(&client_addr), reinterpret_cast<socklen_t *>(&addr_size))) == -1)
		std::cerr << "werbserv: [emerg]: class Socket: accept_connection: accept failed" << std::endl;
	return client_socket;
}

void *handle_connection(void *a)
{
	Thread *t;

	t = reinterpret_cast<Thread*>(a);
	try
	{
		Request request(t->client_socket, t->server);
		if (ISERROR(request.get_error().first) == false)
		{
			Response a(t->client_socket, request, t->server);
			a.execute();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: [warn]: class Socket: handle_connection: " << e.what() << '\n';
	}
	delete t;
	return NULL;
}

int Socket::create_thread(int & client_socket, Server & server)
{
	pthread_t id;
	Thread *t;

	try
	{
		t = new Thread;
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: [warn]: class Socket: handle_connection: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
	t->init(server, client_socket);
	pthread_create(&id, NULL, handle_connection, t);
	_threads_id.push_back(id);
	return EXIT_SUCCESS;
}

int Socket::doublon_ports(std::vector<Server> & servers)
{
	std::vector<Server>::iterator it = servers.begin(), ite = servers.end();
	std::vector<Port>::iterator begin, end;
	std::map<Port, int> list;

	while (it != ite)
	{
		std::vector<Port> &tmp = it->get_port();
		if (tmp.size() == 0)
		{
			std::cerr << "webserv: [emerg]: class Socket: doublon_ports: Server without port" << "\n";
			return EXIT_FAILURE;
		}
		begin = tmp.begin();
		end = tmp.end();
		while (begin != end)
		{
			if (list.insert(std::make_pair(*begin, 0)).first == list.end())
			{
				std::cerr << "webserv: [emerg]: class Socket: doublon_ports: Servers with same port" << "\n";
				return EXIT_FAILURE;
			}
			++begin;
		}
		++it;
	}
	return EXIT_SUCCESS;
}