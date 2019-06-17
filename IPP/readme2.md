# Implementační dokumentace k 2. úloze do IPP 2018/2019

>**Jméno a příjmení:** Adam Sedláček  
**Login:** xsedla1e

## Implementace interpret.py
### Overeni a seřazení
Prvně ověřím všechny přepínače programu. Bud se vypíše daný požadavek např. help nebo chyba. Načtu celé XML a seřadím podle `order` a `argX`, tak abych měl order od 1 do N a totéž platí i pro argumenty. Pokud ani jedno není ve správné posloupnosti, tak ukončím s chybou 32.

### Spravnost syntaxe
Poté co jsem si seřadil XML, jsem znovu procyklil a ověřuji zda `opcode` sedí s počtem argumentů. Taktéž ověřuji zda i něco není navíc.
Pomocí regexů ověřím i zda sedí daný typ (var, sym), který může být u instrukce.

### Hlavní tělo intepretu
U ověření syntaxe jsem si už dané instrukce ukládal do stuktury a danou strukturu do pole _instrukci_. Z té pak čerpám a jen pro daný opcode volám příslušnou funkci, která zařídí správnou interpretaci. Poté je téměř každá funkce téměř stejná, ověření zda se jedná o kompatibilní typ, případně _vytažení_ hodnoty z proměnné. Při jakémkoliv problému nebo šáhnutí do špatného rámce, vyvolám výjimku, kterou následně odchytím a vypíšu v jaké funkci nastala chyba a jaký druh chyby např. špatné typy při sčítání, dělení 0, mimo index u `GETCHAR`, atd.

---
## Implementace test.php
### Parsování argumentů
Vytvořil jsem si pomocné pole `$switches`, kde mám výchozí hodnoty. Poté pouze procyklím přes `$argv` a přes regex zjistím o jaké argumenty se jedná. Pak už mám jen _strom_ podmínek, které mi zavolají podle těchto přepínačů správné otestování.

### do_test_parse
Využívám věstavěné funkce `glob()`, která dokáže najít všechny soubory ve složce se zadanou koncovkou. Pak testuji (zápis, vytvoření, čtení) a případně generuji soubory s `*.rc, *.out` kde vytvářím i dočasný `*.xml`, který následně mažu, taktéž i `*.log`, který je tvořen `jexamxml.jar`, který plní i funkci srovnání `xml` souborů.

### do_test_interpret
Téměř totožná funkce jako `do_test_parse()` jen lehce upravená se souborem `*.in` a na rozdíl `*.out` využívám linuxový `diff`.

### do_test_both
V podstatě jen zkombinováno `do_test_parse` s `do_test_interpret`, kde jsem se rozhodl, že pokud test _selže_ v parseru, tak jej dál netestuji na shodu kódu, protože ze zadání to není jasné co se má udát a nemá smysl testovat dál kód, který se nikdy za hranici chyby nedostane a mohl by být tudíž po chybě značně rozdílný.

### print_html_header
Tato funkce slouží čistě na výtiskl `HTML/CSS`. Mám zde i procentuální zastoupení úspěšnosti testu a pro lepší čitelnost jsem se rozhodnul zvýraznit sudé řádky jinou barvou pozadí, taktéž pro lepší čitelnost jsem využil zažitých zvyklostí *červená - špatná* a *zelená - dobrá* jestli test prošel (OK) nebo ne (FAILED).

### do_main_stuff
_Hlavní_ funkce, která zajištuje správný výpočet procent, které testy prošly a vytváří sudé/liché řádky pro `HTML` a správně uzavře `print_html_header`.