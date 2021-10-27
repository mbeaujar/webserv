#include "Request.hpp"
#include "../socket.hpp"

int atoi(std::string str, int index);

int first_line(std::string & request, Request & r, int i) 
{
    r.set_path(request.substr(i, skip_word_request(request, i) - i));
    while (request[i] && request[i] != '\n')
        i++;
    if (request[i])
        i++;
    return i;
}


// Request parse_body(std::string header) 


Request parse_header(std::string request) {
    
    int i = 0;
	int host = 0;
	int method = 0;
    Request r;
	
    while (request[i]) {
        if (request.compare(i, 3, "GET") == 0) {
			if (method > 0) {
		        std::cerr << "Error: too many methods" << std::endl;
				r.set_error(std::make_pair(400, "Bad request"));
            }
            r.set_method(GET);
            i += 4;
            i = first_line(request, r, i);
			method++;
        }
        else if (request.compare(i, 4, "POST") == 0) {
            if (method > 0) {
		        std::cerr << "Error: too many methods" << std::endl;
				r.set_error(std::make_pair(400, "Bad request"));
            }
			r.set_method(POST);
            i += 5;
            i = first_line(request, r, i);
			method++;
        }
        else if (request.compare(i, 6, "DELETE") == 0) {
            if (method > 0) {
		        std::cerr << "Error: too many methods" << std::endl;
				r.set_error(std::make_pair(400, "Bad request"));
            }
			r.set_method(DELETE);
            i += 7;
            i = first_line(request, r, i);
			method++;
        }
        else if (request.compare(i, 5, "Host:") == 0) {
            i += 6;
            r.set_host(request.substr(i, skip_word_request(request, i) - i));
            i = skip_word_request(request, i);
			if (method == 0) {
		        std::cerr << "Error: host before method" << std::endl;
				r.set_error(std::make_pair(400, "Bad request"));
            }
			host++;
        }
        else if (request.compare(i, 15, "Content-Length:") == 0) {
            i += 16;
            atoi(request, i);
            i = skip_word_request(request, i);
        }
        else if (request.compare(i, 13, "Content-Type:") == 0) {
            i += 14;
            r.set_content_type(request.substr(i, skip_word_request(request, i) - i));
            i = skip_word_request(request, i);
        } 
		else if (request.compare(i, 18, "Transfer-Encoding:") == 0) {  // CHUNKED GZIP
			i += 19;
			while (request[i] && request[i] != '\n') {
				r.add_transfer_encoding(request.substr(i, skip_word_request(request, i) - i));
				i = skip_word_request(request, i);
				if (request[i] != '\n')
					i++;
			}
		}
		else if (request.compare(i, 5, "date:") == 0) {
			i += 5;
			r.set_date(request.substr(i, skip_word_request(request, i) - i));
			i = skip_word_request(request, i); 
		}
        else {
            while (request[i] && request[i] != '\n')
                i++;
            if (request[i]) 
                i++;
        }
    }
    if (r.get_host().empty() || host > 1) {
		std::cerr << "Error: none or too many Host field" << std::endl;
        r.set_error(std::make_pair(400, "Bad request"));
	}
	if (r.get_path().length() > 2048) {
		std::cerr << "Error: path to long (more than 2048 characters" << std::endl;
        r.set_error(std::make_pair(400, "Bad request"));
	}
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

void    print_request(Request & a) {

    std::cout << "----------- Request -----------" << std::endl;
    std::cout << " -> Host     : -> " << a.get_host()           << std::endl;
    std::cout << " -> Type     : -> " << a.get_content_type()   << std::endl;
    std::cout << " -> Path     : -> " << a.get_path()           << std::endl;
    std::cout << " -> Method   : -> " << a.get_method()         << std::endl;
    std::cout << " -> Length   : -> " << a.get_content_length() << std::endl;
    // std::cout << " -> Encoding : -> " << a.get_transfer_encoding() << std::endl;
    std::cout << " -> Error    : -> " << a.get_error().first << " -> " << a.get_error().second << std::endl;
    std::cout << " -> Return   : -> " << a.get_return().first << " -> " << a.get_return().second << std::endl;
    std::cout << "-------------------------------" << std::endl;
}
