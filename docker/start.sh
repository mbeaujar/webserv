#!/bin/bash


docker run -it -p 80:80 -v /home/user42/Bureau/webserv/docker/conf:/etc/nginx/sites-available/ web