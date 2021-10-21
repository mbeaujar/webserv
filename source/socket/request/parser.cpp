#include "Request.hpp"

// GET /index.html HTTP/1.1
// Host: localhost
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
// Accept-Encoding: gzip, deflate

int first_line(std::string & request, Request & r, int i) 
{
    r.set_path(request.substr(i, skip_word_exception(request, i) - i));
    i += skip_word(request, i);
    return i;
}

Request parse_request(std::string request) {
    
    Request r;

    for (int i = 0; request[i]; i++) {

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
    }
}