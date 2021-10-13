/**
 * @file create_server.cpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <iostream>
#include "Server.hpp"
#include "../prototype.hpp"


// 1 root max

// 1 location avec le même chemin max 

// error_page illimité -- si il y a 2 error_page c'est le premier dans le fichier qui compte



Server create_server(std::string file) 
{
	Server a;

	for (int i = 0; file[i]; i++)
	{
		if (file[i] == '#')
			i = skip_comment(file, i);
		i = skip_space(file, i);
		// if listen

		// if index
		if (file.compare(i, 5, "index")) {
			i += 5;
			parse_elements(file, i, a);
		}

		// if server_name
		if (file.compare(i, 11, "server_name")) {
			i += 11;
			parse_elements(file, i, a);
		}

		// if root
		if (file.compare(i, 4, "root")) {
			i += 4;
			parse_root(file, i, a);
		}
		
		// if error_page
		if (file.compare(i, 10, "error_page")) {
			i += 10;
			parse_error_page(file, i, a);
		}

		// if client_max_body_size




		// if autoindex 


		// if location
		if (file.compare(i, 8, "location")) {
			i += 8;
			while (file[i] && file[i] != '{') { // skip space + comment
				if (isspace(file[i]))
					i = skip_space(file, i);
				else if (file[i] == '#')
					i = skip_comment(file, i);
				else 
					throw std::invalid_argument("Wrong file 11");
			}
			int end = skip_bracket(file, i);
			// i = debut 
			// end = fin
		}

	}
}