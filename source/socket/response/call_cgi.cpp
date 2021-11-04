#include "../socket.hpp"
#include <sys/wait.h>
#include <stdlib.h>

// NE PAS SUPPRIMER

// https://www.htmlhelp.com/faq/cgifaq.2.html
// https://www.oreilly.com/openbook/cgi/ch03_02.html
// https://www.w3.org/Protocols/HTTP/Object_Headers.html#date

char	*dupnormi(const char *s1)
{
	int		i;
	char	*s2;

	i = 0;
	while (s1[i])
		i++;
	try {
		s2 = new char[i + 1];
	} catch (std::bad_alloc) {
		std::cerr << "Error: bad_alloc dupnormi" << std::endl;
		return NULL;
	}
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

void free_tab(char **envp, int len) {
	if (envp == NULL)
		return;
	for (int i = 0; i < len; i++) {
		if (envp[i])
			delete [] envp[i];
	}
	delete [] envp;
}

std::string clean_cgi(int fd_in, int fd_out, char **argv = NULL, char **envp = NULL) {
	if (fd_in != -1)
		close(fd_in);
	if (fd_out != -1)
		close(fd_out);
	free_tab(argv, 2);
	free_tab(envp, 6);
	return "";
}

int create_temporary_file(std::string filename) {
	int fd = -1;

	if (file_exist(filename))
		fd = open(filename.c_str(), O_RDWR);
	else
		fd = open(filename.c_str(), O_CREAT | O_RDWR, S_IRWXO);
	if (fd == -1)
		std::cerr << "Error: can't create file " << filename << ": " << strerror(errno) << std::endl;
	return fd;
}

char **create_envp(Request & request, std::string & method, std::string & path_to_cgi, std::string & path) {
	char **envp;
	
	try {
		envp = new char*[7];
	} catch(std::bad_alloc) {
		std::cerr << "Error: bad_alloc in create_envp" << std::endl;
		return NULL;
	}
	std::string only_path_cgi = cut_path(path_to_cgi);

	envp[0] = dupnormi(std::string("REQUEST_METHOD=" + method).c_str());
	envp[1] = dupnormi(std::string("SERVER_PROTOCOL=HTTP/1.1").c_str());
	envp[2] = dupnormi(std::string("PATH_INFO=" + only_path_cgi).c_str());
	envp[3] = dupnormi(std::string("QUERY_STRING=" + request.get_query_string()).c_str());
	envp[4] = dupnormi(std::string("REQUEST_URI=" + only_path_cgi).c_str());
	envp[5] = dupnormi(std::string("SCRIPT_FILENAME=" + path).c_str());
	envp[6] = 0;
	for (int i = 0; i < 6; i++) {
		if (!envp[i]) {
			free_tab(envp, 6);
			return NULL;
		}
	}
	return envp;
}

char **create_argv(std::string & path_to_cgi, std::string & path) {
	char **argv;
	
	try {
		argv = new char*[3];
	} catch (std::bad_alloc) {
		std::cerr << "Error: bad_alloc in create_argv" << std::endl;
		return NULL; 
	}
	argv[0] = dupnormi(cut_filename(path_to_cgi).c_str());
	if (argv[0] == NULL) {
		delete [] argv;
		std::cerr << "Error: cgi malloc for argv" << std::endl;
		return NULL;
	}
	argv[1] = dupnormi(path.c_str());
	if (argv[1] == NULL) {
		delete argv[0];
		delete [] argv;
		std::cerr << "Error: cgi malloc for argv" << std::endl;
		return NULL;
	}
	argv[2] = 0;
	return argv;
}

std::string call_cgi(Request & request, int client_socket, std::string path_to_file, std::string method, std::string path_to_cgi) {
	std::string body;
	int fd_in, fd_out;
	char **argv = NULL, **envp = NULL;
	int pid, status;
	std::string path_in, path_out;
	
	if (method == "GET")
		path_in = path_to_file;
	else 
		path_in = ".in_" + to_string(client_socket);
	path_out = ".out_" + to_string(client_socket);
	fd_in = create_temporary_file(path_in);
	fd_out = create_temporary_file(path_out);
	if (fd_in == -1 || fd_out == -1)
		return clean_cgi(fd_in, fd_out);
	if ((argv = create_argv(path_to_cgi, path_to_file)) == NULL)
		return clean_cgi(fd_in, fd_out, argv);
	if ((envp = create_envp(request, method, path_to_cgi, path_to_file)) == NULL)
		return clean_cgi(fd_in, fd_out, argv, envp);
	pid = fork();
	if (pid == -1)
		return clean_cgi(fd_in, fd_out, argv, envp);
	if (pid == 0) {
		dup2(fd_in, 0);
		dup2(fd_out, 1);
		execve(path_to_cgi.c_str(), argv, envp);
		clean_cgi(fd_in, fd_out, argv, envp);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	body = get_file_content(path_out);
	clean_cgi(fd_in, fd_out, argv, envp);
	if (method == "POST")
		remove_file(path_in.c_str());
	remove_file(path_out.c_str());
	return body;
}
