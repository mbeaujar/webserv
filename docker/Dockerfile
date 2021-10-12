FROM debian:buster


RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y nginx

# /etc/nginx/sites-available/default.conf

EXPOSE 80

ENTRYPOINT exec nginx -g 'daemon off;'