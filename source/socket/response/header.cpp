#include "../socket.hpp"

std::string allow_method(Request & request) {
	std::string method;
	struct s_method m = request.get_methods();

	if (m.m_get == true)
		method += " GET";
	if (m.m_delete == true) {
		if (method != "")
			method += ",";
		method += " DELETE";
	}
	if (m.m_post == true) {
		if (method != "")
			method += ",";
		method += " POST";
	}
	return method;
}

std::string hour_date() {
	std::string	date;

	char 		buf[1000];
	char 		savedlocale[256];
	time_t		now = time(0);
	struct tm tm = *gmtime(&now);

	strcpy(savedlocale,  setlocale(LC_ALL, NULL));
	setlocale(LC_ALL, "C");
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	setlocale(LC_ALL, savedlocale);

	date = buf;
	return date;
}

std::string get_last_modified() 
{
	std::string date;
	char 		buf[1000];
	struct stat		stats;
	struct tm		*tm;
	
	tm = gmtime(&stats.st_mtime);
	strftime(buf, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	date = std::string(buf);

	return date;
}

std::string get_file_content(std::string filename) 
{
    std::string	    content, line;
    std::ifstream	file;

    file.open(filename.c_str());
    while (std::getline(file, line)) {
        content.append(line);
		if (!file.eof())
			content.append("\n");
	}
	file.close();
    return content;
}

std::string header(Request & request) {
	std::string header;
	std::pair<int, std::string> redirect = request.get_return();

	if (redirect.first != -1)
		request.set_error(std::make_pair(redirect.first, "Moved Permanently"));
	header += "HTTP/1.1 " + to_string(request.get_error().first) + " " + request.get_error().second + "\n";
	header += "Date: " + hour_date() + "\n";
	header += "Server: " + request.get_host() + "\n";
	header += "Content-length: " + to_string(request.get_content_length()) + "\n";
	// header += "Last-Modified: ";
	if (request.get_error().first == 405)
		header += "Allow:" + allow_method(request) + "\n";
	if (redirect.first != -1)
		header += "Location: " + redirect.second + "\n";
	header += "\r\n"; // blank line
	return header;
}
