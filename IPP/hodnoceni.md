# Hodnocení pro IPP (co došlo emailem)

## Parser.php
Projekt byl nejdříve rozbalen a následně spuštěn s řadou hodnotících testů.
Testy jsou rozděleny do několika kategorií. Podle výsledků
jednotlivých testů byla stanovena procentuální úspěšnost Vašeho skriptu
v jednotlivých kategoriích a z těchto hodnot odvozeny body. 
Do WIS se importují body zaokrouhlené na jedno desetinné místo.
Testovací příklady nebudou uveřejněny. 

Hodnocení je individuálně vygenerováno pro každého studenta, doplněno o ručně vložené připomínky a následně 
automaticky rozesláno na školní email.

Hodnocené části (nehodnocené části jsou vynechány):
 1) Automatické testy parse.php - základní.
 2) Automatické testy parse.php - registrovaná rozšíření.
 3) Automatické testy test.php, pokud byl součástí archivu:
      včetně ručního zhodnocení kvalit vytvořených HTML reportů.
 4) Případné malusy a bonusy (pozdní odevzdání, příspěvky na fóru, ...).
 5) Hodnocení dokumentace readme1(.pdf/.md) a štábní kultury zdrojových kódů (především komentářů).

    Následuje seznam zkratek, které se mohou vyskytnout v komentářích k hodnocení test.php:
      OK = výstupy byly v pořádku
      NO = žádné smysluplné výsledky
      NODIFF = špatné porovnání výstupů
      NOSTOP = nezastavení po chybě v parse.php a následný pokus interpretace nevalidního XML
      NOUTF = chybí podpora Unicode znaků (např. v názvech adresářů a souborů)
      NODETAILS = schází informace o (jednotlivých) testech či jejich počtech
      PARSEONLY = funguje jen --parse-only (výsledky pro ostatní testy jsou většinou špatně)
      WRONG = neodpovídající výsledky testů
      ERRORS = jiné chyby zpracování (např. nenalezení/nezobrazení všech testů)

    Následuje seznam zkratek, které se mohou vyskytnout v komentářích k hodnocení dokumentace a štábní kultury skriptů:
Vysvětlivky zkratek v dokumentaci:
  CH = pravopisné chyby, překlepy
  FORMAT = špatný formát vzhledu dokumentu (nedodrženy požadavky)
  SHORT = nesplňuje minimální požadavky na délku či obsah
  STRUCT = nevhodně strukturováno (např. bez nadpisů)
  MISSING = dokumentace nebyla odevzdána
  STYLE = stylizace vět, nečitelnost, nesrozumitelnost
  COMMENT = chybějící nebo nedostatečné komentáře ve zdrojovém textu
  FILO = nedostatečná filosofie návrhu (abstraktní popis struktury programu, co následuje za čím)
  JAK = technicky nedostatečný popis řešení
  SRCFORMAT = opravdu velmi špatná štábní kultura zdrojového kódu
  SPACETAB (jen pro informaci) = kombinování mezer a tabelátorů k odsazování zdrojového textu
  LANG = míchání jazyků (většinou anglické termíny v českém textu)
  HOV = hovorové nebo nevhodné slangové výrazy
  FORM = nepěkná úprava, nekonzistentní velikost a typ písma apod.
  TERM = problematická terminologie (neobvyklá, nepřesná či přímo špatná)
  IR = nedostatečně popsaná vnitřní reprezentace (např. pro paměť, sekvenci instrukcí apod.)
  PRED (jen pro informaci) = pozor na osamocené neslabičné předložky na konci řádků
  BLOK (jen pro informaci) = chybí zarovnaní do bloku místo méně pěkného zarovnání na prapor (doleva)
  KAPTXT (jen pro informaci) = mezi nadpisem a jeho podnadpisem by měl být vždy nějaký text
  MEZ (jen pro informaci) = za otevírající nebo před zavírající závorku mezera nepatří
  ICH (jen pro informaci) = ich-forma (psaní v první osobě jednotného čísla) není většinou vhodná pro programovou dokumentaci
  SAZBA (jen pro informaci) = alespoň identifikátory proměnných a funkcí se patří sázet písmem s jednotnou šířkou písmen (např. font Courier)
  OK = k dokumentaci byly maximálně minoritní připomínky


V případě nějakých problémů s hodnocením je přiložen
log dekomprimace odevzdáného archivu.

Termíny reklamací budou vypsány ve WIS.


Vaše hodnocení části 1): 5,98 bodů
Vaše hodnocení části 2): 0,92 bodů
Vaše hodnocení části 3): 0,45 bodů
  Komentář hodnocení části 3): NOUTF, NOSTOP, ERRORS, NODETAILS
Vaše hodnocení části 5): 0,80 bodů
  Komentář hodnocení části 5) (sražka uváděny v minibodech, 1 bod = 100 minibodů): CH (-10) preklepy a chyby (“Já se rozhodl”), STYLE (-10) občas ťažko čitateľné, chýbajúca interpunkcia, neobvyklé zvraty

Pokud jste obdrželi výsledek částí 1) mimo hodnotící interval, tak
bude oříznut, tak že získáte za implementaci alespoň 0 a ne více jak maximum bodů za daný skript.


Procentuální hodnocení jednotlivých kategorií skriptu parse.php: 
Lexikální analýza: 97%
Syntaktická analýza: 100%
Zpracování instrukcí: 98%
Zpracování programů: 100%
Rozšíření STATP 92%
Celkem bez rozšíření: 98%

## interpret.py a test.php

Následující procentuální hodnocení je FINÁLNÍ hodnocení Vaší úlohy č. 2.

Projekt byl nejdříve rozbalen a následně spuštěn s řadou hodnotících testů.
Testy jsou rozděleny do několika kategorií. Podle výsledků
jednotlivých testů byla stanovena procentuální úspěšnost Vašeho skriptu
v jednotlivých kategoriích a z těchto hodnot odvozeny body. 
Do WIS se importují body zaokrouhlené na jedno desetinné místo.
Testovací příklady nebudou uveřejněny. 

Hodnocení je individuálně vygenerováno pro každého studenta, doplněno o ručně vložené připomínky a následně 
automaticky rozesláno na školní email.

Hodnocené části (nehodnocené části jsou vynechány):
 1) Automatické testy interpret.py - základní.
 2) Automatické testy interpret.py - registrovaná rozšíření.
 3) Automatické testy test.php (základní a registrovaná rozšíření) včetně ručního zhodnocení kvalit vytvořených HTML reportů.
 4) Případné malusy a bonusy (pozdní odevzdání, příspěvky na fóru, ...).
 5) Hodnocení dokumentace readme2(.pdf/.md) a štábní kultury zdrojových kódů (především komentářů).

    Následuje seznam zkratek, které se mohou vyskytnout v komentářích k hodnocení test.php:
      OK = výstupy byly v pořádku
      EMPTY = prázdný nebo téměř prázdný HTML soubor
      INTONLY = fungují vesměs pouze testy s parametrem --int-only (ostatní vesměs nefungují)
      JExamXML = podezření, že výstup při --parse-only nebyl porovnáván pomocí nástroje JExamXML (či se špatným nastavením), ale pouze nástrojem diff
      NO = žádné smysluplné výsledky
      NOINT = nefunguje testování interpretu
      NODIFF = špatné porovnání výstupů
      NODIR/DIRS = v rekurzivním výpisu testů (--recursive) není poznat, který test je z jaké složky
      NOPRE = nefunguje dogenerování souborů s implicitním obsahem
      NOSTOP = nezastavení po chybě v parse.php a následný pokus interpretace nevalidního XML
      NOUTF = chybí podpora Unicode znaků (např. v názvech adresářů a souborů), nebo nefunguje vyhodnocování testů s Unicode znaky v názvech testů/adresářů
      NODETAILS = schází informace o (jednotlivých) testech či jejich počtech
      PARSEONLY = funguje jen vesměs --parse-only (výsledky pro ostatní testy jsou většinou špatně)
      STATS = chybí nejdůležitější statistiky jako například počet provedených testů a počet úspěšných nebo neúspěšných testů
      WARNINGS = varování PHP/Python vložené přímo do výstupního HTML (často nepřehledně)
      WRONG = neodpovídající výsledky testů
      ERRORS = jiné chyby zpracování (např. nenalezení/nezobrazení všech testů nebo nalezení i těch, které se nalézt neměly)

    Následuje seznam zkratek, které se mohou vyskytnout v komentářích k hodnocení dokumentace a štábní kultury skriptů:
Vysvětlivky zkratek v dokumentaci:
  CH = pravopisné chyby, překlepy
  FORMAT = špatný formát vzhledu dokumentu (nedodrženy požadavky)
  SHORT = nesplňuje minimální požadavky na délku či obsah
  STRUCT = nevhodně strukturováno (např. bez nadpisů)
  MISSING = dokumentace nebyla odevzdána
  STYLE = stylizace vět, nečitelnost, nesrozumitelnost
  COMMENT = chybějící nebo nedostatečné komentáře ve zdrojovém textu
  FILO = nedostatečná filosofie návrhu (abstraktní popis struktury programu, co následuje za čím)
  JAK = technicky nedostatečný popis řešení
  SRCFORMAT = opravdu velmi špatná štábní kultura zdrojového kódu
  SPACETAB (jen pro informaci) = kombinování mezer a tabelátorů k odsazování zdrojového textu
  LANG = míchání jazyků (většinou anglické termíny v českém textu)
  HOV = hovorové nebo nevhodné slangové výrazy
  FORM = nepěkná úprava, nekonzistentní velikost a typ písma apod.
  TERM = problematická terminologie (neobvyklá, nepřesná či přímo špatná)
  IR = nedostatečně popsaná vnitřní reprezentace (např. pro paměť, sekvenci instrukcí apod.)
  PRED (jen pro informaci) = pozor na osamocené neslabičné předložky na konci řádků
  BLOK (jen pro informaci) = chybí zarovnaní do bloku místo méně pěkného zarovnání na prapor (doleva)
  KAPTXT (jen pro informaci) = mezi nadpisem a jeho podnadpisem by měl být vždy nějaký text
  MEZ (jen pro informaci) = za otevírající nebo před zavírající závorku mezera nepatří
  ICH (jen pro informaci) = ich-forma (psaní v první osobě jednotného čísla) není většinou vhodná pro programovou dokumentaci
  SAZBA (jen pro informaci) = alespoň identifikátory proměnných a funkcí se patří sázet písmem s jednotnou šířkou písmen (např. font Courier)
  OK = k dokumentaci byly maximálně minoritní připomínky


Termíny reklamací budou vypsány ve WIS (viz termín 'Reklamace hodnocení 2. úlohy').


Vaše hodnocení části 1): 5,11 bodů
Vaše hodnocení části 2): 0,00 bodů
Vaše hodnocení části 3): 2,35 bodů
  Komentář hodnocení části 3): NOPRE, JExamXML?
  Body za rozšíření k části 3): 0,00 bodů
Vaše hodnocení části 5): 1,75 bodů
  Komentář hodnocení části 5) (sražka uváděny v minibodech, 1 bod = 100 minibodů): ICH (-0), CH (-15) (“výtiskl”, chýbajúce čiarky) STYLE (-10) (občas príliš hovorové, zvláštne formulácie ako “protože ze zadání to není jasné co se má udát”)

Pokud jste obdrželi výsledek částí 1) mimo hodnotící interval, tak
bude oříznut, tak že získáte za implementaci alespoň 0 a ne více jak maximum bodů za daný skript.

Dekomprimace archivu proběhla úspěšně.

Procentuální hodnocení jednotlivých kategorií skriptu interpret.py: 
Lexikální analýza: 100%
Syntaktická analýza: 100%
Sémantická analýza (detekce chyb): 100%
Běhové chyby (detekce): 72%
Interpretace instrukcí: 64%
Interpretace programů: 0%
Rozšíření FLOAT 0%
Rozšíření STACK 0%
Rozšíření STATI 0%
Celkem bez rozšíření: 61%

