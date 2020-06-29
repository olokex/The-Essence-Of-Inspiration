#!/bin/bash

# Jestli /root/.my.cnf existuje tak nepotrebujeme heslo
if [ -f /root/.my.cnf ]; then
	mysql -e "use cinema; UPDATE user SET role_id=1 WHERE email='admin@cinema.cz';"
    mysql -e "use cinema; UPDATE user SET role_id=2 WHERE email='redaktor@cinema.cz';"
    mysql -e "use cinema; UPDATE user SET role_id=3 WHERE email='pokladni@cinema.cz';"
    mysql -e "use cinema; UPDATE user SET role_id=4 WHERE email='divak@cinema.cz';"
else
    echo "Zadejte prosim heslo root uzivatele od MySQL. :)"
    echo "Pozn. heslo je skryte a nejspise ho bude potreba zadat opakovane."
    read -sp rootpasswd
	mysql -u root -p${rootpasswd} -e "use cinema; UPDATE user SET role_id=1 WHERE email='admin@cinema.cz';"
    mysql -u root -p${rootpasswd} -e "use cinema; UPDATE user SET role_id=2 WHERE email='redaktor@cinema.cz';"
    mysql -u root -p${rootpasswd} -e "use cinema; UPDATE user SET role_id=3 WHERE email='pokladni@cinema.cz';"
    mysql -u root -p${rootpasswd} -e "use cinema; UPDATE user SET role_id=4 WHERE email='divak@cinema.cz';"
fi
