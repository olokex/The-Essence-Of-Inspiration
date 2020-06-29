-------------------------------------------------------------------------------------------------------------------------------
-- xsedla1e & xsadil07
-- 2. ukol do IDS, 2018/2019
-------------------------------------------------------------------------------------------------------------------------------

DROP TABLE Účet CASCADE CONSTRAINTS;
DROP TABLE Správce CASCADE CONSTRAINTS;
DROP TABLE Uživatel CASCADE CONSTRAINTS;
DROP TABLE Místnost CASCADE CONSTRAINTS;
DROP TABLE Oddělení CASCADE CONSTRAINTS;
DROP TABLE Technika CASCADE CONSTRAINTS;
DROP TABLE Oprava CASCADE CONSTRAINTS;
DROP TABLE Žádost CASCADE CONSTRAINTS;
DROP TABLE Spravuje CASCADE CONSTRAINTS;
DROP SEQUENCE auto_increment_seq;

CREATE TABLE Účet (
    ID INT NOT NULL PRIMARY KEY,
    Jméno VARCHAR2(127) NOT NULL,
    Příjmení VARCHAR2(127) NOT NULL,
    Rodné_číslo INT NOT NULL,
    Email VARCHAR2(127) NOT NULL,
    Telefon INT NOT NULL,
    Město VARCHAR2(127) NOT NULL,
    Ulice VARCHAR2(127) NOT NULL,
    PSC INT NOT NULL,
    ID_Umístění INT NOT NULL,
    CHECK (regexp_like(Jméno, '^[a-zA-Z-]+$')),
    CHECK (regexp_like(Email, '^[a-zA-Z0-9._%-]+@[a-zA-Z0-9._%-]+\.[a-zA-Z]{2,4}$')),
    CHECK (regexp_like(Rodné_číslo, '^[0-9]{2}[0-1][0-9][0-3][0-9][0-9]{4}$')),
    CHECK (regexp_like(PSC, '^[0-9]{5}$')),
    CHECK (regexp_like(Telefon, '^[0-9]{9,}$'))
);

CREATE TABLE Správce (
  Účet INT NOT NULL PRIMARY KEY,
  Licence VARCHAR2(127) NOT NULL
);

CREATE TABLE Uživatel (
  Účet INT NOT NULL PRIMARY KEY
);

CREATE TABLE Místnost (
    Číslo_místnosti INT NOT NULL PRIMARY KEY,
    Patro INT NOT NULL,
    Typ VARCHAR2(1024) NOT NULL,
    Popis VARCHAR2(2048) NOT NULL,
    ID_Oddělení INT NOT NULL
);

CREATE TABLE Oddělení(
    Číslo_oddělení INT NOT NULL PRIMARY KEY,
    Popis VARCHAR2(2048) NOT NULL
);

CREATE TABLE Technika (
    ID INT NOT NULL PRIMARY KEY,
    Výrobní_číslo VARCHAR2(127) NOT NULL,
    Typ VARCHAR2(1024) NOT NULL,
    Výrobce VARCHAR2(127) NOT NULL,
    Model_techniky VARCHAR2(127) NOT NULL,
    Specifikace VARCHAR2(1024) NOT NULL,
    ID_Umístění INT NOT NULL,
    ID_Uživatele INT NOT NULL
);

CREATE TABLE Oprava (
    ID INT NOT NULL PRIMARY KEY,
    Kdy DATE NOT NULL,
    Aktuální_stav VARCHAR2(1024) NOT NULL,
    ID_Techniky INT NOT NULL,
    ID_Správce INT
);

CREATE TABLE Žádost (
    ID INT NOT NULL PRIMARY KEY,
    Kdy DATE NOT NULL,
    ID_Techniky INT NOT NULL,
    ID_Opravy INT,
    ID_Správce INT NOT NULL,
    ID_Uživatele INT NOT NULL
);


-- Vztahy
CREATE TABLE Spravuje (
  ID INT NOT NULL PRIMARY KEY,
  ID_Správce INT NOT NULL,
  ID_Techniky INT NOT NULL
);

ALTER TABLE Účet
  ADD CONSTRAINT FK_UmístěníÚčtu
    FOREIGN KEY (ID_Umístění) REFERENCES Místnost(Číslo_místnosti);

ALTER TABLE Správce
  ADD CONSTRAINT FK_SprávcovskýÚčet
    FOREIGN KEY (Účet) REFERENCES Účet(ID) ON DELETE CASCADE;

ALTER TABLE Uživatel
  ADD CONSTRAINT FK_UživatelskýÚčet
    FOREIGN KEY (Účet) REFERENCES Účet(ID) ON DELETE CASCADE;

ALTER TABLE Místnost
  ADD CONSTRAINT FK_OdděleníMístnosti
    FOREIGN KEY (ID_Oddělení) REFERENCES Oddělení(Číslo_oddělení);

ALTER TABLE Technika
  ADD CONSTRAINT FK_UmístěníTechniky
    FOREIGN KEY (ID_Umístění) REFERENCES Místnost(Číslo_místnosti)
  ADD CONSTRAINT FK_UživatelTechniky
    FOREIGN KEY (ID_Uživatele) REFERENCES Uživatel(Účet);

ALTER TABLE Oprava
  ADD CONSTRAINT FK_OpravaTechniky
    FOREIGN KEY (ID_Techniky) REFERENCES Technika(ID) ON DELETE CASCADE
  ADD CONSTRAINT FK_SprávceOpravy
    FOREIGN KEY (ID_Správce) REFERENCES Správce(Účet) ON DELETE SET NULL;

ALTER TABLE Žádost
  ADD CONSTRAINT FK_ŽádostTechniky
    FOREIGN KEY (ID_Techniky) REFERENCES Technika(ID) ON DELETE CASCADE
  ADD CONSTRAINT FK_ŽádostOpravy
    FOREIGN KEY (ID_Opravy) REFERENCES Oprava(ID) ON DELETE SET NULL
  ADD CONSTRAINT FK_SprávceŽádosti
    FOREIGN KEY (ID_Správce) REFERENCES Správce(Účet)
  ADD CONSTRAINT FK_PodavatelŽádosti
    FOREIGN KEY (ID_Uživatele) REFERENCES Uživatel(Účet);

ALTER TABLE Spravuje
  ADD CONSTRAINT FK_Spravuje
    FOREIGN KEY (ID_Správce) REFERENCES Správce(Účet) ON DELETE CASCADE
  ADD CONSTRAINT FK_SpravovanáTech
    FOREIGN KEY (ID_Techniky) REFERENCES Technika(ID) ON DELETE CASCADE;

---------- TRIGGERY ----------

-- Autoinkrement PK pro tabulku s ucty.
CREATE SEQUENCE auto_increment_seq START WITH 1;
CREATE OR REPLACE TRIGGER auto_increment
  BEFORE INSERT ON Účet
  FOR EACH ROW
BEGIN
  SELECT auto_increment_seq.nextval
  INTO :new.ID
  FROM dual;
END;
/

-- Zkontroluje platne stavy oprav (tj. hotovo, probiha, nevyrizeno)
-- Neni uvedeno demonstracni predvedeni, aby skript nevyvolal chybu.
CREATE OR REPLACE TRIGGER kontrola_stavu
  BEFORE INSERT OR UPDATE OF Aktuální_stav ON Oprava
  FOR EACH ROW
BEGIN
  IF :NEW.Aktuální_stav NOT IN ('Hotovo', 'Probíhá', 'Nevyřízeno') THEN
    RAISE_APPLICATION_ERROR(-20000, 'Neplatny stav opravy.');
  END IF;
END;
/

---------- PROCEDURY ----------

-- Procedura vypise jmeno zadaneho mesta a procentualni zastoupeni v bydlistich uzivatelu.
CREATE OR REPLACE PROCEDURE zastoupeni_mesta(mesto IN VARCHAR2)
IS
  CURSOR zaznamy IS SELECT * FROM Účet;   -- Nacteme zaznamy
  polozka zaznamy%ROWTYPE;                -- Polozka zaznamu
  pocet NUMBER;                           -- Pocitadlo shod
  celkem NUMBER;                          -- Celkovy pocet zaznamu
BEGIN
  pocet := 0;                             -- Vynulujeme
  celkem := 0;
  OPEN zaznamy;
  LOOP
    FETCH zaznamy INTO polozka;           -- Nacteme polozku
    EXIT WHEN zaznamy%NOTFOUND;           -- Ukoncujici podminka
    IF (polozka.Město = mesto) THEN       -- Shoda
      pocet := pocet + 1;
    END IF;
    celkem := celkem + 1;
  END LOOP;
  DBMS_OUTPUT.PUT_LINE('Zastoupeni mesta ' || mesto || ' je ' || pocet/celkem*100 || '%.');

  EXCEPTION
    WHEN ZERO_DIVIDE THEN
      DBMS_OUTPUT.PUT_LINE('Zastoupeni mesta ' || mesto || ' je 0%.');
    WHEN OTHERS THEN
      RAISE_APPLICATION_ERROR(-20206, 'Nastala chyba pri provadeni procedury.');
END;
/

-- Vypise seznam uctu, kteri maji emailovy ucet u zadane domeny + celkovy a procentualni pocet.
CREATE OR REPLACE PROCEDURE ucty_s_email_domenou(domena IN VARCHAR2)
IS
  CURSOR zaznamy IS SELECT * FROM Účet;   -- Nacteme zaznamy
  zaznam zaznamy%ROWTYPE;                 -- Polozka zaznamu
  pocet NUMBER;
  celkem NUMBER;                          -- Celkovy pocet zaznamu
  sub VARCHAR2(100);                      -- Promenna pro ulozeni domeny
BEGIN
  celkem := 0;                            -- Vynulujeme
  pocet := 0;
  OPEN zaznamy;
  LOOP
    FETCH zaznamy INTO zaznam;            -- Nacteme polozku
    EXIT WHEN zaznamy%NOTFOUND;           -- Ukoncujici podminka
    sub := SUBSTR(zaznam.Email, INSTR(zaznam.Email, '@') + 1, LENGTH(zaznam.Email));
    IF (sub = domena) THEN                -- Shoda
      DBMS_OUTPUT.PUT_LINE(zaznam.Jméno || ' ' || zaznam.Příjmení);
      pocet := pocet + 1;
    END IF;
    celkem := celkem + 1;
  END LOOP;
  DBMS_OUTPUT.PUT_LINE('Celkem: ' || pocet || ', procentualne: ' || pocet/celkem*100 || '%.');

  EXCEPTION
    WHEN ZERO_DIVIDE THEN
      DBMS_OUTPUT.PUT_LINE('Celkem 0, procentualne 0%.');
    WHEN OTHERS THEN
      RAISE_APPLICATION_ERROR(-20206, 'Nastala chyba pri provadeni procedury.');
END;
/


---------- VLOZENI DAT ----------

-- Vlozeni oddeleni
INSERT INTO Oddělení (Číslo_oddělení, Popis)
    VALUES (1, 'CVT');
INSERT INTO Oddělení (Číslo_oddělení, Popis)
    VALUES (2, 'Kanceláře');
INSERT INTO Oddělení (Číslo_oddělení, Popis)
    VALUES (3, 'Přednášky');

-- Vlozeni mistnosti
INSERT INTO Místnost (Číslo_místnosti, Patro, Typ, Popis, ID_Oddělení)VALUES
    (10, 1, 'Správa CVT', 'Místnost slouží pro zaměstnance správy CVT', 1);
INSERT INTO Místnost (Číslo_místnosti, Patro, Typ, Popis, ID_Oddělení)VALUES
    (20, 1, 'Serverovna', 'V místnosti jsou uloženy veškeřé servery', 1);
INSERT INTO Místnost (Číslo_místnosti, Patro, Typ, Popis, ID_Oddělení)VALUES
    (30, 2, 'Kancelář ředitele', 'Zde sídlí velký šéf', 2);
INSERT INTO Místnost (Číslo_místnosti, Patro, Typ, Popis, ID_Oddělení)VALUES
    (40, 2, 'Hlavní aula', 'Pro technické konference', 3);

-- Vlozeni uctu
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Jiří', 'Novák', 5905127215, 'example0@seznam.com', 4558414875, 'Brno', 'Zahradní 15', 44739, 10);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Jan', 'Svoboda', 8112317343, 'example1@email.com', 410585496, 'Ostrava', 'Krátká 15', 79652, 10);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Petr', 'Novotný', 1906212050, 'example2@email.com', 421608504, 'Plzeň', 'Nádražní 15', 25439, 10);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Josef', 'Dvořák', 3605184726, 'example3@email.com', 501080405, 'Liberec', 'Školní 15', 25586, 10);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Pavel', 'Černý', 9407101671, 'example4@gmail.com', 900512650, 'Olomouc', 'Polní 15', 64744, 20);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Martin', 'Procházka', 7310288605, 'example5@email.com', 121125560, 'Budějovice', 'Luční 15', 72848, 20);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Jaroslav', 'Kučera', 2601098059, 'example6@xmail.com', 660356276, 'Pardubice', 'Komenského 15', 63404, 30);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Tomáš', 'Veselý', 1401136966, 'example7@email.com', 931258512, 'Králové', 'Nová 15', 46203, 10);
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Miroslav', 'Horák', 6605251305, 'example8@fitmail.com', 720762065, 'Pardubice', 'Husova 15', 60765, 10);

INSERT INTO Správce (Účet, Licence) VALUES (1, 'Elektronika');
INSERT INTO Správce (Účet, Licence) VALUES (2, 'Sítě');
INSERT INTO Správce (Účet, Licence) VALUES (3, 'Hardware');
INSERT INTO Správce (Účet, Licence) VALUES (4, 'Grafika');
INSERT INTO Uživatel (Účet) VALUES (5);
INSERT INTO Uživatel (Účet) VALUES (6);
INSERT INTO Uživatel (Účet) VALUES (7);
INSERT INTO Uživatel (Účet) VALUES (8);
INSERT INTO Uživatel (Účet) VALUES (9);

-- Vlozeni techniky
INSERT INTO Technika (ID, Výrobní_číslo, Typ, Výrobce, Model_techniky, Specifikace, ID_Umístění, ID_Uživatele)
    VALUES (1, '7FC0TI3231', 'PC', 'Asus', 'KLM18', '4 Ghz, RAM 124 KB...', 10, 5);
INSERT INTO Technika (ID, Výrobní_číslo, Typ, Výrobce, Model_techniky, Specifikace, ID_Umístění, ID_Uživatele)
    VALUES (2, 'M381KHSV4I', 'Tablet', 'Lenovo', 'LKJS48LS', '4 Ghz, RAM 124 KB...', 30, 6);
INSERT INTO Technika (ID, Výrobní_číslo, Typ, Výrobce, Model_techniky, Specifikace, ID_Umístění, ID_Uživatele)
    VALUES (3, 'QGE87D3J8K', 'Monitor', 'Dell', '23FHDIPS', 'Pekny monitor se stojankem', 20, 7);
INSERT INTO Technika (ID, Výrobní_číslo, Typ, Výrobce, Model_techniky, Specifikace, ID_Umístění, ID_Uživatele)
    VALUES (4, 'A4J734U0ME', 'Monitor', 'Dell', '24FHDTN', 'Ne tak pekny monitor bez stojanku', 10, 8);
INSERT INTO Technika (ID, Výrobní_číslo, Typ, Výrobce, Model_techniky, Specifikace, ID_Umístění, ID_Uživatele)
    VALUES (5, '54J61LE55B', 'Myš', 'Hama', 'SCRL456', 'Obyčejná PC myš', 20, 9);

-- Vlozeni oprav
INSERT INTO Oprava (ID, Kdy, Aktuální_stav, ID_Techniky, ID_Správce)
    VALUES (1, '20.4.2019', 'Probíhá', 3, 3);
INSERT INTO Oprava (ID, Kdy, Aktuální_stav, ID_Techniky, ID_Správce)
    VALUES (2, '1.4.2019', 'Hotovo', 1, 1);
INSERT INTO Oprava (ID, Kdy, Aktuální_stav, ID_Techniky, ID_Správce)
    VALUES (3, '10.4.2019', 'Probíhá', 2, 2);
INSERT INTO Oprava (ID, Kdy, Aktuální_stav, ID_Techniky, ID_Správce)
    VALUES (4, '18.3.2019', 'Hotovo', 4, 1);

-- Vlozeni zadosti
INSERT INTO Žádost (ID, Kdy, ID_Techniky, ID_Opravy, ID_Správce, ID_Uživatele)
    VALUES (1, '1.4.2019', 3, 1, 2, 5);
INSERT INTO Žádost (ID, Kdy, ID_Techniky, ID_Opravy, ID_Správce, ID_Uživatele)
    VALUES (2, '2.4.2019', 1, 2, 3, 6);
INSERT INTO Žádost (ID, Kdy, ID_Techniky, ID_Opravy, ID_Správce, ID_Uživatele)
    VALUES (3, '2.4.2019', 1, 2, 1, 7);

-- Spravuje
INSERT INTO Spravuje (ID, ID_Správce, ID_Techniky)
    VALUES (1, 1, 1);
INSERT INTO Spravuje (ID, ID_Správce, ID_Techniky)
    VALUES (2, 2, 2);
INSERT INTO Spravuje (ID, ID_Správce, ID_Techniky)
    VALUES (3, 3, 3);
INSERT INTO Spravuje (ID, ID_Správce, ID_Techniky)
    VALUES (4, 4, 4);
INSERT INTO Spravuje (ID, ID_Správce, ID_Techniky)
    VALUES (5, 1, 5);

---------- SELECTY ----------

-- Umístění uživatelů amístností, kteří jsou v prvním patře a nejmenují se Pavel (spojení dvou tabulek)
SELECT U.Jméno, U.Příjmení, M.Číslo_místnosti AS místnost
FROM Účet U
  INNER JOIN Místnost M ON U.ID_Umístění = M.Číslo_místnosti
WHERE M.Patro = 1 AND Jméno <> 'Pavel';

-- Výpis jmen uživatelů, kteří bydlí v Pardubicích, a kteří vlastní techniku (spojení dvou tabulek)
SELECT T.Typ, T.Výrobce, U.Jméno, U.Příjmení
FROM Účet U
  INNER JOIN Technika T ON U.ID = T.ID_Uživatele
WHERE U.Město = 'Pardubice';

-- Vypsání monitorů v opravě včetně správce, který za ní zodpovídá (spojení tří tabulek)
SELECT O.Aktuální_stav, T.Výrobní_číslo, T.Typ, T.Výrobce, U.Jméno AS Jméno_správce, U.Příjmení AS Příjmení_správce
FROM Technika T
  INNER JOIN Oprava O ON T.ID = O.ID_Techniky
  INNER JOIN Účet U ON O.ID_Správce = U.ID
WHERE T.Typ = 'Monitor';

-- Vypis počtů technik stejného typu s počtem větším než 1 (dotaz s klauzulí GROUP BY a agregační funkcí)
SELECT Typ, COUNT(*) AS Počet
FROM Technika
GROUP BY Typ
HAVING COUNT(*) > 1;

-- Počet lidí v jednotlivých místnostech v prvním patře (dotaz s klauzulí GROUP BY a agregační funkcí)
SELECT M.Číslo_místnosti, COUNT(*) AS Počet_osob
FROM Účet U INNER JOIN Místnost M ON U.ID_Umístění = M.Číslo_místnosti
WHERE M.Patro = 1
GROUP BY M.Číslo_místnosti;

-- Výpis techniky, která má žádost na opravu (dotaz obsahující predikát EXISTS)
SELECT T.Typ, T.Výrobce, T.Model_techniky
FROM Technika T
WHERE EXISTS(
  SELECT *
  FROM Žádost Z
  WHERE T.ID = Z.ID_Techniky);

-- Výpis uživatelů, kteří podali žádost mezi datem 2.4.2019 a 8.4.2019 (dotaz s predikátem IN a vnořeným selectem)
SELECT U.Jméno, U.Příjmení, U.Email
FROM Účet U
WHERE U.ID IN(
  SELECT Z.ID_Uživatele
  FROM Žádost Z
  WHERE U.ID = Z.ID_Uživatele AND Z.KDY BETWEEN '2-4-2019' AND '8-4-2019');


---------- UDELENI PRAV DRUHEMU CLENU SKUPINY ----------
GRANT ALL ON Účet TO xsedla1e;
GRANT ALL ON Správce TO xsedla1e;
GRANT ALL ON Uživatel TO xsedla1e;
GRANT ALL ON Místnost TO xsedla1e;
GRANT ALL ON Oddělení TO xsedla1e;
GRANT ALL ON Technika TO xsedla1e;
GRANT ALL ON Oprava TO xsedla1e;
GRANT ALL ON Žádost TO xsedla1e;
GRANT ALL ON Spravuje TO xsedla1e;

GRANT EXECUTE ON zastoupeni_mesta TO xsedla1e;
GRANT EXECUTE ON ucty_s_email_domenou TO xsedla1e;

---------- EXPLAIN PLAN ----------
DROP INDEX idx;

EXPLAIN PLAN FOR    -- Bez indexu
SELECT M.Číslo_místnosti, COUNT(*) AS Počet_osob
FROM Účet U INNER JOIN Místnost M ON U.ID_Umístění = M.Číslo_místnosti
WHERE M.Patro = 1
GROUP BY M.Číslo_místnosti;
SELECT *
FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX idx ON Místnost (Číslo_místnosti, Patro);

EXPLAIN PLAN FOR    -- S indexem
SELECT M.Číslo_místnosti, COUNT(*) AS Počet_osob
FROM Účet U INNER JOIN Místnost M ON U.ID_Umístění = M.Číslo_místnosti
WHERE M.Patro = 1
GROUP BY M.Číslo_místnosti;
SELECT *
FROM TABLE(DBMS_XPLAN.display);


---------- MATERIALIZOVANY POHLED ----------
DROP MATERIALIZED VIEW ÚčetInfo;

CREATE MATERIALIZED VIEW LOG ON Účet WITH PRIMARY KEY, ROWID(Město) INCLUDING NEW VALUES;
CREATE MATERIALIZED VIEW ÚčetInfo
CACHE                       -- Optimalizace cteni z pohledu
BUILD IMMEDIATE             -- Naplneni pohledu hned po jeho vytvoreni
REFRESH FAST ON COMMIT      -- Aktualizace ihned po commitu
ENABLE QUERY REWRITE        -- Bude pouzivany optimalizatorem
AS SELECT Město, COUNT(*)
FROM Účet
GROUP BY Město;

GRANT ALL ON ÚčetInfo TO xsedla1e;

-- Ukazka materializovaneho pohledu
SELECT * FROM ÚčetInfo;
INSERT INTO Účet (Jméno, Příjmení, Rodné_číslo, Email, Telefon, Město, Ulice, PSC, ID_Umístění)
    VALUES ('Rick', 'Grimes', 8604127995, 'rick@gmail.com', 105489075, 'Brno', 'Nová 18', 44734, 10);
COMMIT;
SELECT * FROM ÚčetInfo;


---------- DEMONSTRACE PROCEDUR ----------
-- 1. PROCEDURA
call zastoupeni_mesta('Pardubice');
-- Vypise:
-- Zastoupeni mesta Pardubice je 20%.

-- 2. PROCEDURA
call ucty_s_email_domenou('email.com');
-- Jan Svoboda
-- Petr Novotný
-- Josef Dvořák
-- Martin Procházka
-- Tomáš Veselý
-- Celkem: 5, procentualne: 50%.
