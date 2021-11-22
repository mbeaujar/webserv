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
	//char 		savedlocale[256];
	time_t		now = time(0);
	struct tm tm = *gmtime(&now);

	//strcpy(savedlocale,  setlocale(LC_ALL, NULL));
	//setlocale(LC_ALL, "C");
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S GMT", &tm);
	//setlocale(LC_ALL, savedlocale);

	date = buf;
	return date;
}

std::string get_last_modified(std::string path) {
	std::string 	date;
	struct stat		stats;
	struct tm		*tm;
	char 			buf[1000];
	
	memset(&stats, 0, sizeof(stats));
	stat(path.c_str(), &stats);
	tm = gmtime(&stats.st_mtime);
	strftime(buf, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	date = std::string(buf);
	return date;
}

std::string get_file_content(std::string filename) {
    std::string	    content, line;
    std::ifstream	file;

    file.open(filename.c_str());
	if (file.is_open() == false) {
		std::cerr << "Error: can't open the file: " << filename << std::endl;
		return "";
	}
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
	header += "HTTP/1.1 " + to_string(request.get_error().first) + " " + request.get_error().second + "\r\n";
	header += "Date: " + hour_date() + "\r\n";
	header += "Server: " + request.get_host() + "\r\n";
	if (request.get_content_type().empty() == false)
		header += "Content-Type: " + request.get_content_type() + "\r\n";
	if (request.get_method() == GET)
		header += "Content-length: " + to_string(request.get_content_length()) + "\r\n";
	if (file_exist(request.get_path()))
		header += "Last-Modified: " + get_last_modified(request.get_path()) + "\r\n";
	if (request.get_error().first == 405)
		header += "Allow:" + allow_method(request) + "\r\n";
	if (redirect.first != -1)
		header += "Location: " + redirect.second + "\r\n";
	header += "\r\n"; // blank line
	return header;
}
