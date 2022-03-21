#include "Socket.hpp"
#include "prototype.hpp"

void debug(std::vector<Server> &servers);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "number of arguments invalid"
                  << "\n";
        return EXIT_FAILURE;
    }
    try
    {
        std::vector<Server> servers;
        servers = parser(argv[1]);
        // debug(servers);

        Socket sockets;

        sockets.execute(servers);
    }
    catch (std::exception &e)
    {
        std::cerr << "webserv: [emerg] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// ----------------------- UTILS ---------------------- //

std::string extension(std::string &ext)
{
    int i = ext.length() - 1;

    while (i > 0 && ext[i] != '.')
        --i;
    if (i == 0)
        return "";
    return ext.substr(i, ext.length() - i);
}

void debug(std::vector<Server> &servers)
{
    std::vector<Server>::iterator it = servers.begin(), ite = servers.end();
    while (it != ite)
    {
        printserver(*it);
        ++it;
    }
}

bool file_exist(std::string &filename)
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

bool is_directory(std::string &path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
            return true;
    }
    return false;
}

std::string get_file_content(std::string filename)
{
    std::ifstream file;

    file.open(filename.c_str());
    if (file.is_open() == true)
    {
        std::ostringstream contents;
        contents << file.rdbuf();
        file.close();
        return contents.str();
    }
    std::cerr << "Error: can't open the file: " << filename << "\n";
    return "";
}

int skip_line(std::string &line, int i)
{
    while (line[i] && line[i] != '\n')
    {
        if (line[i] == '\r')
            if (line[i + 1] && line[i + 1] == '\n')
                break;
        ++i;
    }
    return i;
}

/*int skip_semicolon(std::string & line, int i)
{
    while (line[i] && line[i] != ';')
        ++i;
    return i;
}*/