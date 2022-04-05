
pwdesc=$(echo $PWD | sed 's_/_\\/_g')

sed -i 's/_path/'$pwdesc'/g' config_files/default.conf

sed -i 's_\\/_/_g' config_files/default.conf
