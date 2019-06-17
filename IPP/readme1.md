# Implementační dokumentace k 1. úloze do IPP 2018/2019

>**Jméno a příjmení:** Adam Sedláček  
**Login:** xsedla1e

## Main
Abych nemusel mít neustále ve while cyklu podmínku jestli se už načetla hlavička `.IPPcode19` nebo ne. Rozhodnul jsem se, že vždy první řádek musí takto začínat. V zadání není specifikováno zda se před hlavičkou mohou nacházet prázdné řádky nebo komentáře, tudíž toto beru jako chybu.

Pro XML generování využívám již zabudovanou knihovnu [DomDocument](http://php.net/manual/en/class.domdocument.php). Kde vytvořím nobý objekt a nastavím atributy/elementy, tak jak požaduje zadání.

Každá instrukce začíná na novém řádku a oddělena nenulovým počtem [bílých znaků](https://en.wikipedia.org/wiki/Whitespace_character), tudíž vždy načtu řádek přes `$line = fgets(STDIN)`, který následně zpracuju pomocí regexu, který mi zajistí, že veškeré [bílé znaky](https://en.wikipedia.org/wiki/Whitespace_character) změní na pouze jednu mezeru. Takto _převedený_ řádek zbavím komentáře najitím prvního výskytu `#`, pokud se toto povedlo mám dvou prvkové pole, a můžu tedy přičíst ke statistice komentářů. Druhou část ignoruji a pracuji pouze s první polovinou, kterou následně zbavím přebytečných mezer na začátku a konci. Takto _očištěný_ řádek předám funkci `process_line`, pokud tato funkce úspěšně skončí, tak se vytiskne výsledné XML.

### Process_line
Je jednoduchá funkce, která vezme přijatou řádku a rozdělí ji podle mezer do pole. Podle počtu prvků v poli se přes switch rozhodne, která funkce se zavolá. Případně se vypíše chyba na `STDERR` a parser skončí s chybou 23.

### Instruction
Je série funkcí, které se vždy volají z `process_line`. Tyto funkce přijímají pole stringů. Na první pozici, by vždy měla být instrukce, která se převede pomocí `strtoupper()` a ověří pomocí podmínky, pokud by se instrukce neseděla s počtem argumentů, tak se ověří, zda je v celkovém seznamu instrukcí (uživatel si jen popletl počet argumentů dané instrukce, ale syntax je správně) dá vědět uživateli a skončí se s chybou opcode 22. Pokud by se nenacházela v seznamu instrukcí, tak se opět vypíše chyba a ukončí se s chybou syntaxe 23. Pokud instrukce je korektní, přejde se k dalšímu ověření argumentů od žádného až po tři. Dle specifikace ze zadání pro typy, symboly... Argumenty testuji postupně pomocí regexu, tudíž tu by šlo udělat i rozšíření, které by přímo uživateli dalo vědět, že bylo očekáváno něco zcela jinýho např. uživatel zadal `MOVE string@var LF@a`, zde by se mohlo napsat, že první argumunet musí být proměnná. Pokud ověření opcodu a argumentů prošlo, je následně zavolána funkce na přidání elementu do XML.

### Instruction_xml
Opět série funkcí, které přijmou už přímo opcode/typ/návěští v parametrech. Pomocí podmínek se rozřadí a správně vytvoří element/y pro XML, kde se přidá atribut a hodnota dle specifik ze zadání.

### Rozšíření STATP
Udělal jsem si vlastní parsování parametrů z příkazové řádky, protože `getopt` mi přišlo neintuitivní a ze zadání není jasné, zda při dvojím zadání např. `--jumps` má být chyba nebo ne. Já se rozhodl, že to jako chybu brát nebudu a pokud si uživatel z nějakého důvodu přeje mít tento údaj dvakrát, tak jej dvakrát dostane. Pak už jednoduše přičítám ke globálním čítačům při procházení `IPPcode19` a následný tisk přes switch.