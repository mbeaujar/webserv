export REQUEST_METHOD=GET
export SCRIPT_NAME=www/query_string.php 
export SERVER_PROTOCOL=HTTP/1.1 
export GATEWAY_INTERFACE=CGI/1.1 
export PATH_INFO=/query_string.php
export REQUEST_URI=/query_string.php
export PATH_TRANSLATED=www/query_string.php 
export QUERY_STRING="last_name=test" 
export SCRIPT_FILENAME=www/query_string.php
export CONTENT_TYPE="text/html" 
export CONTENT_LENGTH=392
echo "last_name=test" | ./ubuntu_cgi_tester