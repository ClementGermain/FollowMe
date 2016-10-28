#!/bin/bash

read -p 'ID : ' login_etud
read -p 'PWD : ' -s password_etud

curl -silent --data-urlencode "auth_user=$login_etud" \
	--data-urlencode "auth_pass=$password_etud" \
	--data-urlencode "accept=Connection" \
	https://portail-invites-lan.insa-toulouse.fr:8003 \
	| grep 'LogoutWin' > out.txt

if test -s out.txt
then
	printf "\n\033[0;32mConnected! :)\033[0m\n"
else
	printf "\n\033[0;31mError! :(\033[0m\n"
fi

rm out.txt
