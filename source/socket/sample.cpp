#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sstream>
#include <cstring>
#include <string>
#include <errno.h>
#include <arpa/inet.h>
#include <fstream>

#include <stropts.h>
#include <poll.h>

#define PORT 8080

// socket     -- int socket(int domain, int type, int protocol);  
// setsockopt -- int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
// bind       -- int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// listen     -- int listen(int sockfd, int backlog);
// accept     -- int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
// send       -- size_t send(int sockfd, const void *buf, size_t len, int flags);

template <typename tostring>
std::string to_string(tostring n)
{
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

std::string readFile(std::string filename) 
{
    std::string txt, line;
    std::ifstream file;

    file.open(filename.c_str());
    if (!file.is_open()) {
            std::cout << "Filename can't be opened" << std::endl;
            return "0";
    }
    while (std::getline(file, line))
        txt+=line + "\n";
    std::cout << "code html: " << std::endl;
    std::cout << txt << std::endl;
    return (txt);
}

int main()
{
    int                 fd_socket;
    int                 fd_accept;
    int                 opt = 1;
    struct sockaddr_in  my_addr;
    int             addrlen = sizeof(my_addr);
    char requete[1024] = {0};

    struct pollfd read_poll;
    struct pollfd send_poll;
    int timeout_msecs = 500;
    int ret;

    /* Open STREAMS device. */
    send_poll.events = POLLIN | POLLWRBAND;
    send_poll.events = POLLOUT | POLLWRBAND;

    // socket : socket() creates an endpoint for 
    // communication and returns a file descriptor that refers to that endpoint.

    // An endpoint is a remote computing device that communicates back and forth with a network to which it is connected
    if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "problème avec le socket: " << strerror(errno) <<  std::endl;
        return 1;
    }
	
    read_poll.fd = fd_socket; // open("/dev/dev0", ...);

    if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        std::cout << "problème avec le setsockopt: " << strerror(errno) << std::endl;
        close(fd_socket);
        return 1;
    }

    my_addr.sin_port        = htons(PORT); // port
    my_addr.sin_family      = AF_INET;     // protocole IPV4
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // specify the ip adress
    // INADDR_ANY == inet_addr("0.0.0.0") == ((in_addr_t) 0x00000000) --- inet_addr("127.0.0.1") ==  ((in_addr_t) 0x7f000001)

    // bind : When a socket is created with socket(2), it exists in a name space 
    // (address family) but has no address assigned to it.
    // bind() assigns the address specified by addr to the 
    // socket referred to by the file descriptor sockfd.
    if (bind(fd_socket, (struct sockaddr*)&my_addr, addrlen) == -1) {
        std::cout << "problème avec le bind: " << strerror(errno) << std::endl;
        close(fd_socket);
        return 1;
    }

    if (listen(fd_socket, 10) == -1) {
        std::cout << "problème avec le listen: " << strerror(errno) << std::endl;
        close(fd_socket);
        return 1;
    }
    

    if ((fd_accept = accept(fd_socket,  (struct sockaddr*)&my_addr, (socklen_t*)&addrlen)) == -1) {
        std::cout << "problème avec accept: " << strerror(errno) << std::endl;
        close(fd_socket);
        return 1;
    }
    read_poll.fd = fd_accept; // open("/dev/dev1", ...);
    

    ret = poll(&read_poll, 2, timeout_msecs);
    if (ret > 0) {
        if (read(fd_accept, requete, 1024) == -1) {
            std::cout << "problème avec le read: " << strerror(errno) << std::endl;
            close(fd_socket);
            close(fd_accept);
            return 1;
    	}
    }

    std::cout << "requête: \n" << requete << "\n"; 

    std::string content = readFile("index.html");

    std::string reponse = "HTTP/1.1 200 OK\n";

    reponse += "\n"; // line between header and content

    reponse += content;
    ret = poll(&send_poll, 2, timeout_msecs);
    if (ret > 0 && send(fd_accept, reponse.c_str(), reponse.length(), 0) == -1) {
        std::cout << "problème avec le send: " << strerror(errno) << std::endl;
        close(fd_accept);
        close(fd_socket);
        return 1;
    }

    close(fd_accept);
    close(fd_socket);
    return 0;
}