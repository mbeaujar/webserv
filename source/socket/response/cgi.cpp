#include "../socket.hpp"
#include <sys/wait.h>
#include <stdlib.h>

void free_tab(char **tab, int j) {
	if (!tab)
		return;
	for (int i = 0; i < j; i++) {
		if (tab[i])
			delete tab[i];
	}
	delete [] tab;
}

std::string clean(int fd_in, int fd_out, char **envp, char **argv, std::string body) {
	if (fd_in != -1)
		close(fd_in);
	if (fd_out != -1)
		close(fd_out);
	free_tab(envp, 6);
	free_tab(argv, 3);
	return body;
}

int delete_temporary_file(std::string filename, int client_socket) {
	filename += to_string(client_socket);
	int error = remove(filename.c_str());
	if (error == -1)
		std::cerr << "Error: can't remove file for client: " << client_socket << "\t" << strerror(errno) << std::endl;
	return error;
}

int create_temporary_file(std::string filename, int client_socket) {
	int fd;
	filename += to_string(client_socket);
	if (file_exist(filename))
		remove(filename.c_str());
	fd = open(filename.c_str(), O_CREAT | O_RDWR, S_IRWXU);
	if (fd == -1) 
		std::cerr << "Error: create file for client: " << client_socket << "\t" << strerror(errno) << std::endl;
	return fd;
}

char **create_envp(Request & request, std::string method, std::string fastcgi) {
	char **envp = new char*[6];
	envp[0] = strdup(std::string("REQUEST_METHOD=" + method).c_str());
	envp[1] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	envp[2] = strdup(std::string("PATH_INFO=" + cut_path(fastcgi)).c_str());
	envp[3] = strdup(std::string("SCRIPT_NAME=" + cut_filename(fastcgi)).c_str());
	envp[4] = strdup(std::string("QUERY_STRING=" + request.get_query_string()).c_str());
	envp[5] = 0;
	for (int i = 0; i < 5; i++) {
		std::cout << "ENVP[" << i << "] : " << envp[i] << std::endl;
		if (!envp[i]) {
			free_tab(envp, 5);
			return NULL;
		}
	}
	return envp;
}

char **create_argv(std::string fastcgi, std::string path) {
	char **argv = new char*[3];
	std::cout << "ICIIII: " << path << std::endl;
	argv[0] = strdup(fastcgi.c_str());
	argv[1] = strdup(path.c_str());
	argv[2] = 0;
	for (int i = 0; i < 2; i++) {
		if (!argv[i]) {
			free_tab(argv, 2);
			return NULL;
		}
	}
	return argv;
}

std::string call_cgi(Request & request, std::string fastcgi, int client_socket, std::string body, std::string method, std::string path) {
	int fd_out = -1, fd_in = -1;
	char **envp = NULL, **argv = NULL;
	int pid, status;
	
	fd_in = create_temporary_file(".in_", client_socket);
	fd_out = create_temporary_file(".out_", client_socket);
	if (fd_in == -1 || fd_out == -1)
		return clean(fd_in, fd_out, envp, argv, body);
	write(fd_in, body.c_str(), body.length());
	if ((envp = create_envp(request, method, fastcgi)) == NULL)
		return clean(fd_in, fd_out, envp, argv, body);
	if ((argv = create_argv(fastcgi, path)) == NULL)
		return clean(fd_in, fd_out, envp, argv, body);
	std::cout << "ARG: " << argv[1] << std::endl;
	pid = fork();
	if (pid == -1) 
		return clean(fd_in, fd_out, envp, argv, body);
	if (pid == 0) {
		dup2(fd_in, 0);
		dup2(fd_out, 1);
		execve(fastcgi.c_str(), argv, NULL);
		clean(fd_in, fd_out, envp, argv, body);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	clean(fd_in, fd_out, envp, argv, body);
	std::cerr << "DEBUG CHILD STATUS: " << WEXITSTATUS(status) << std::endl;
	std::string newfile = get_file_content(".out_" + to_string(client_socket));
	// delete_temporary_file(".in_", client_socket);
	// delete_temporary_file(".out_", client_socket);
	return newfile;
}
