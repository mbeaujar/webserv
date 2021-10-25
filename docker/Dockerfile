FROM debian:buster

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y nginx

# /etc/nginx/sites-available/default.conf

RUN chmod -R 777 /var/www/html 
RUN chown -R www-data:www-data /var/www/html

EXPOSE 80

ENTRYPOINT exec nginx -g 'daemon off;' & bash