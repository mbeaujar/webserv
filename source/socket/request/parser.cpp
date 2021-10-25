#include "Request.hpp"
#include "../socket.hpp"

int atoi(std::string str, int index);

int first_line(std::string & request, Request & r, int i) 
{
    r.set_path(request.substr(i, skip_word_exception(request, i) - i));
    while (request[i] && request[i] != '\n')
        i++;
    if (request[i])
        i++;
    return i;
}


// Request parse_header(std::string header) 


Request parse_request(std::string request) {
    
    int i = 0;
    Request r;

    while (request[i]) {

        if (request.compare(i, 3, "GET") == 0) {
            r.set_method(GET);
            i += 4;
            i = first_line(request, r, i);
        }
        else if (request.compare(i, 4, "POST") == 0) {
            r.set_method(POST);
            i += 5;
            i = first_line(request, r, i);
        }
        else if (request.compare(i, 6, "DELETE") == 0) {
            r.set_method(DELETE);   
            i += 7;
            i = first_line(request, r, i);
        }
        else if (request.compare(i, 5, "Host:") == 0) {
            i += 6;
            r.set_host(request.substr(i, skip_word_exception(request, i) - i));
            i = skip_word(request, i);
        }
        else if (request.compare(i, 15, "Content-Length:") == 0) {
            i += 16;
            atoi(request, i);
            i = skip_word(request, i);
        }
        else if (request.compare(i, 13, "Content-Type:") == 0) {
            i += 14;
            r.set_content_type(request.substr(i, skip_word_exception(request, i) - i));
            i = skip_word(request, i);
        }
        else {
            while (request[i] && request[i] != '\n')
                i++;
            if (request[i]) 
                i++;
        }
    }
    if (r.get_host().empty()) // + si il y en a plusieurs
        r.set_error(std::make_pair(400, "Bad request"));
	return r;
}

int     atoi(std::string str, int index) {
    
    int i = 0;
    int res = 0;
    while (isdigit(str[index + i])) {
        res *= 10;
        res += str[index + i] - 48;
        i++;
    }
    return res;
}

void    print_request(Request const & a) {

    std::cout << "----- Request -----" << std::endl;
    std::cout << " -> Host   :" << a.get_host()           << std::endl;
    std::cout << " -> Path   :" << a.get_path()           << std::endl;
    std::cout << " -> Method :" << a.get_method()         << std::endl;
    std::cout << " -> Type   :" << a.get_content_type()   << std::endl;
    std::cout << " -> Length :" << a.get_content_length() << std::endl;
    std::cout << "-------------------" << std::endl;
}
