#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include "../request/Request.hpp"

bool file_exist(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str());
	if (file)
	{
		file.close();
		return true;
	}
	file.close();
	return false;
}

int skip_space(std::string & file, int i) {
	while (file[i] && isspace(file[i]))
		i++;
	return i;
}

int recup_nb(std::string & file, int & i) {
	int port = 0;

	while (isdigit(file[i])) {
		port = (port * 10) +  (file[i] - 48);
		i++;
	}
	return port;
}

bool is_directory(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return true;
	}
	return false;
}


// // VIRE
// int set_file_content(std::string & filename, std::string &content)  // POST !
// {
// 	std::ofstream file(filename.c_str(), std::ios_base::app);

// 	if (file.is_open() == false)
// 		return EXIT_FAILURE;
// 	file << content;
// 	file.close();
// 	return EXIT_SUCCESS;
// }

// // VIRE
std::string & get_file_content(std::string filename)
{
	std::string content;
	std::ifstream file;

	file.open(filename.c_str());
	if (file.is_open() == false)
	{
		std::cerr << "Error: can't open the file: " << filename << std::endl;
		return content; // == return ""
	}
	std::ostringstream contents;
	contents << file.rdbuf();
	file.close();
	content = contents.str();
	return content;
}

std::string path_filename(std::string path)
{
	int i = path.length();
	while (i > 0 && path[i] != '/')
		i--;
	std::string filename = path.substr(i + 1, path.length() - (i + 1));
	if (filename.empty() == true)
		return "/";
	return filename;
}

std::string path_dir(std::string path)
{
	int i = path.length();
	while (i > 0 && path[i] != '/')
		i--;
	return path.substr(0, i);
}

int remove_file(char const * path)
{
	int ret = remove(path);
	if (ret == -1)
		std::cerr << "webserv: [warn]: remove_file: remove fail for path: " << path << std::endl;
	return ret;
}

void print_request(Request & a)
{

	std::cout << "----------- Request -----------" << std::endl;
	std::cout << " -> Host     : -> " << a.get_host() << std::endl;
	std::cout << " -> Type     : -> " << a.get_content_type() << std::endl;
	std::cout << " -> Path     : -> " << a.get_path() << std::endl;
	std::cout << " -> Method   : -> " << a.get_method() << std::endl;
	std::cout << " -> Length   : -> " << a.get_content_length() << std::endl;
	// std::cout << " -> Encoding : -> " << a.get_transfer_encoding() << std::endl;
	std::cout << " -> Error    : -> " << a.get_error().first << " -> " << a.get_error().second << std::endl;
	std::cout << " -> Return   : -> " << a.get_return().first << " -> " << a.get_return().second << std::endl;
	std::cout << " -> Query_string : -> " << a.get_query_string() << std::endl;
	std::cout << "-------------------------------" << std::endl;
}
