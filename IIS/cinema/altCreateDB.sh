#!/bin/bash

# Heslo uzivatele
PASSWDDB="cinema"

# Jmeno DB a uzivatele
MAINDB="cinema"

# Jestli /root/.my.cnf existuje tak nepotrebujeme heslo
if [ -f /root/.my.cnf ]; then
	mysql -e "CREATE DATABASE ${MAINDB} /*\!40100 DEFAULT CHARACTER SET utf8 */;"
	mysql -e "CREATE USER ${MAINDB}@localhost IDENTIFIED BY '${PASSWDDB}';"
	mysql -e "GRANT ALL PRIVILEGES ON ${MAINDB}.* TO '${MAINDB}'@'localhost';"
        mysql -e "FLUSH PRIVILEGES;"
else
	echo "Zadejte prosim heslo root uzivatele od MySQL. :)"
        echo "Pozn. heslo je skryte a nejspise ho bude potreba zadat opakovane."
        read -sp rootpasswd
        mysql -u root -p${rootpasswd} -e "CREATE DATABASE ${MAINDB} /*\!40100 DEFAULT CHARACTER SET utf8 */;"
        mysql -u root -p${rootpasswd} -e "CREATE USER ${MAINDB}@localhost IDENTIFIED BY '${PASSWDDB}';"
        mysql -u root -p${rootpasswd} -e "GRANT ALL PRIVILEGES ON ${MAINDB}.* TO '${MAINDB}'@'localhost';"
        mysql -u root -p${rootpasswd} -e "FLUSH PRIVILEGES;"
fi
echo "Vytvarim tabulky.."
console/index.php orm:schema-tool:create
if [ -f /root/.my.cnf ]; then
	mysql -e "
                use cinema;

                INSERT INTO cinema.role (id, title) VALUES (1, 'Admin');
                INSERT INTO cinema.role (id, title) VALUES (2, 'Redaktor');
                INSERT INTO cinema.role (id, title) VALUES (3, 'Pokladní');
                INSERT INTO cinema.role (id, title) VALUES (4, 'Divák');

                INSERT INTO cinema.type (title) VALUES ('Film');
                INSERT INTO cinema.type (title) VALUES ('Divadlo');
                INSERT INTO cinema.type (title) VALUES ('Přednáška');

                INSERT INTO cinema.genre (title) VALUES ('Komedie');
                INSERT INTO cinema.genre (title) VALUES ('Drama');
                INSERT INTO cinema.genre (title) VALUES ('Historie');
                INSERT INTO cinema.genre (title) VALUES ('Životopisný');
                INSERT INTO cinema.genre (title) VALUES ('Thriller');
                INSERT INTO cinema.genre (title) VALUES ('Horor');
                INSERT INTO cinema.genre (title) VALUES ('Akční');
                INSERT INTO cinema.genre (title) VALUES ('Dobrodružný');
                INSERT INTO cinema.genre (title) VALUES ('Detektivní');
                INSERT INTO cinema.genre (title) VALUES ('Kriminální');
                INSERT INTO cinema.genre (title) VALUES ('Hudební');
                INSERT INTO cinema.genre (title) VALUES ('Katastrofický');
                INSERT INTO cinema.genre (title) VALUES ('Postapokaliptický');
                INSERT INTO cinema.genre (title) VALUES ('Sci-fi');
                INSERT INTO cinema.genre (title) VALUES ('Rodinný');
                INSERT INTO cinema.genre (title) VALUES ('Dětský');
                INSERT INTO cinema.genre (title) VALUES ('Pohádka');
                INSERT INTO cinema.genre (title) VALUES ('Fantasy');
                INSERT INTO cinema.genre (title) VALUES ('Adaptace');
                INSERT INTO cinema.genre (title) VALUES ('Hrdinové');
                INSERT INTO cinema.genre (title) VALUES ('Technologie');
                INSERT INTO cinema.genre (title) VALUES ('Filosofie');
                INSERT INTO cinema.genre (title) VALUES ('Western');
                INSERT INTO cinema.genre (title) VALUES ('Parodie');
                INSERT INTO cinema.genre (title) VALUES ('Milostný');
                INSERT INTO cinema.genre (title) VALUES ('Dokumentární');
                INSERT INTO cinema.genre (title) VALUES ('Animovaný');
                INSERT INTO cinema.genre (title) VALUES ('Sportovní');
                INSERT INTO cinema.genre (title) VALUES ('Tragédie');"
else
        mysql -u root -p${rootpasswd} -e "
                use cinema;

                INSERT INTO cinema.role (id, title) VALUES (1, 'Admin');
                INSERT INTO cinema.role (id, title) VALUES (2, 'Redaktor');
                INSERT INTO cinema.role (id, title) VALUES (3, 'Pokladní');
                INSERT INTO cinema.role (id, title) VALUES (4, 'Divák');
                
                INSERT INTO cinema.type (title) VALUES ('Film');
                INSERT INTO cinema.type (title) VALUES ('Divadlo');
                INSERT INTO cinema.type (title) VALUES ('Přednáška');
                
                INSERT INTO cinema.genre (title) VALUES ('Komedie');
                INSERT INTO cinema.genre (title) VALUES ('Drama');
                INSERT INTO cinema.genre (title) VALUES ('Historie');
                INSERT INTO cinema.genre (title) VALUES ('Životopisný');
                INSERT INTO cinema.genre (title) VALUES ('Thriller');
                INSERT INTO cinema.genre (title) VALUES ('Horor');
                INSERT INTO cinema.genre (title) VALUES ('Akční');
                INSERT INTO cinema.genre (title) VALUES ('Dobrodružný');
                INSERT INTO cinema.genre (title) VALUES ('Detektivní');
                INSERT INTO cinema.genre (title) VALUES ('Kriminální');
                INSERT INTO cinema.genre (title) VALUES ('Hudební');
                INSERT INTO cinema.genre (title) VALUES ('Katastrofický');
                INSERT INTO cinema.genre (title) VALUES ('Postapokaliptický');
                INSERT INTO cinema.genre (title) VALUES ('Sci-fi');
                INSERT INTO cinema.genre (title) VALUES ('Rodinný');
                INSERT INTO cinema.genre (title) VALUES ('Dětský');
                INSERT INTO cinema.genre (title) VALUES ('Pohádka');
                INSERT INTO cinema.genre (title) VALUES ('Fantasy');
                INSERT INTO cinema.genre (title) VALUES ('Adaptace');
                INSERT INTO cinema.genre (title) VALUES ('Hrdinové');
                INSERT INTO cinema.genre (title) VALUES ('Technologie');
                INSERT INTO cinema.genre (title) VALUES ('Filosofie');
                INSERT INTO cinema.genre (title) VALUES ('Western');
                INSERT INTO cinema.genre (title) VALUES ('Parodie');
                INSERT INTO cinema.genre (title) VALUES ('Milostný');
                INSERT INTO cinema.genre (title) VALUES ('Dokumentární');
                INSERT INTO cinema.genre (title) VALUES ('Animovaný');
                INSERT INTO cinema.genre (title) VALUES ('Sportovní');
                INSERT INTO cinema.genre (title) VALUES ('Tragédie');"
fi
