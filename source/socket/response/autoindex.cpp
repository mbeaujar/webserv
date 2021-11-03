#include "../socket.hpp"
#include <sys/types.h>
#include <dirent.h>

// opendir readir

std::string listing(std::string path)
{
    std::string response;
    std::string dirName(path);

    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
	{
        std::cerr << "Error: can't opendir: " << strerror(errno) << std::endl;
        return "";
    }
    // if (dirName[0] != '/')
    //     dirName = "/" + dirName;
    for (struct dirent *forDir = readdir(dir); forDir != NULL; forDir = readdir(dir)) {
        if (strcmp(forDir->d_name, ".") != 0)
        	response += "<a href= \"http://localhost:8080" + cut_filename(dirName) + forDir->d_name + "\">" + forDir->d_name + "</a>\n";
    }
	closedir(dir);
    return response;
}

std::string autoindex_on(std::string path)
{
    std::string response;

    response = "<html>\n";
    response += "<head>\n";
    response += "<title>" + path + "</title>\n";
    response += "</head>\n";
    response += "<body bgcolor=\"white\">\n";
    response += "<h1>Index of " + cut_filename(path) + "</h1>\n";
    response += "<hr>\n";
    response += "<pre>";
    response += listing(path);
    response += "</pre>";
    response += "<hr>\n";
    response += "</body>\n";
    response += "</html>";
    return response;
}
