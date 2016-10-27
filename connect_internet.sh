#!/bin/bash

path=`dirname $0`
file="/log.txt"
date >> $path$file
read -p 'ID : ' login_etud
read -p 'PWD : ' -s password_etud

curl -silent --data-urlencode "auth_user=$login_etud&auth_pass=$password_etud&accept=Connection" https://portail-invites-lan.insa-toulouse.fr:8003 | grep 'LogoutWin' > out.txt
if test -s out.txt
then
echo -e "\nConnected"
else
echo -e "\nError :("
fi
