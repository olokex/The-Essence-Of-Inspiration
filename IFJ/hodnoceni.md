Následující procentuální hodnocení je FINÁLNÍ hodnocení Vašeho projektu.

Projekt byl nejdříve automaticky přeložen a na výsledném binárním souboru byly
spuštěny testy. Testy jsou rozděleny do několika kategorií. Podle výsledků
jednotlivých testů byla stanovena procentuální úspěšnost Vašeho překladače
v jednotlivých kategoriích a z těchto hodnot odvozeny body. 
Do WIS se importují body zaokrouhlené na jednotky.
Testovací příklady nebudou uveřejněny. 

Hodnocení je individuálně vygenerováno pro každého studenta a následně 
automaticky rozesláno na školní email.

Hodnocené části:
 1) Automatické testy a manuální hodnocení implementace:
      zahrnuje automatické testy včetně testů registrovaných rozšíření
               hodnocení implementace předepsané metody syntaktické analýzy,
               dodržení varianty zadání v implementaci (symtable.c),
               přehlednost a kvalita kódu.
    Následuje orientační ceník za nejčastější chyby a prohřešky včetně několika zkratek, které se mohou vyskytovat v komentářích hodnocení implementace:
      NOHEAD = chybí záhlaví ve zdroj. souborech: -0,50 b
      NOAUTHORS = chybí jména autorů v záhlaví: -0,20 b
      PMGP = nadměrné množství globálních proměnných: -0,20 b (tolerována globální proměnná pro chybu, tabulku symbolů a seznam instrukcí)
      HEADCODE = kód v hlavičkových souborech místo v .c souborech: -0,15 b
      LCOM++ = téměř žádné komentáře: -0,30 b
      LCOM = málo komentářů: -0,10 b
      GCOM = příliš mnoho komentářů: -0,10 b
      CLEAN = celková úprava kódu: -0,50 b (odsazení, zakomentované zbytky kódu, ...)
      VUL = vulgarismy: -0,30 b
      NORS/PA = SA analýza chybí: -0,60 b
      NOPSA = SA analýza výrazů chybí: -0,60 b
      SYA = použitý Shunting-Yardův algoritmus: bez postihu
      PC = analýza výrazů pomocí metody precedence-climbing: bez postihu
      IAL+ = TS v jiném souboru než symtable.(c|h): -0,10 b
      NOTS/IAL++ = chybí TS dle zadání: -0,60 b
 2) Prémiové body, korekce, malus: aktivita na fóru, (rychlostní soutěž bude ještě doplněna)
                                   pozdní odevzdání, formální nedostatky.
 3) Dokumentace (viz zadání a seznam zkratek níže).
    Následuje seznam zkratek, které se mohou vyskytnout v komentářích k hodnocení dokumentace:
      KA = chyby v KA, především větší míra nedeterminismu, chybějící důležité lexémy (např. ošetření komentářů atp.)
      strukt. = nedostatky ve struktuře dokumentu (nejčastěji chybějící závěr)
      PSA = chybějící nebo nedostatečný popis precedenční syntaktické analýzy
      RP = chybějící popis rozdělení práce
      GK = chybějící/nedostatečný popis generování kódu
      LL = chyby a nedostatky v LL tabulce
      LLT = chybějící nebo chybná LL tabulka
      typ. = typografické nedostatky (nejčastěji text nebyl zarovnán do bloku)
      ref. = připomínky ke způsobu uvedení zdrojů/literatury (nečíslovaný seznam literatury, neodkazované zdroje)
      SA = nedostatečný/chybějící popis syntaktické analýzy
      SaV = nedostatečný popis syntaktické analýzy výrazů
      PT = chybějící, chybná nebo nečitelná precedenční tabulka
      TS = chybějící/nedostatečný popis tabulky symbolů či její implementace
      gram. = gramatické chyby
    Za většinu prohřešků bylo podle závažnosti strhnuto 0,2 až 0,7 bodu.
    Poznámky k hodnocení dokumentací jsou vesměs řazeny dle klesající závažnosti.


V případě, že nebyl Váš projekt přeložen úspěšně, je přiložen
log dekomprimace odevzdáného archivu a kompilace projektu.

Termíny reklamací budou vypsány ve WIS. V případě snahy o elektronické řešení před řádným termínem pište nejlépe ještě dnes! Bez získaného zápočtu není možné přijít na zkoušku.


Vaše hodnocení části 1): 14,22 bodů
  Komentář hodnocení části 1): LCOM
Vaše hodnocení části 2): 1 bodů
  Komentář hodnocení části 2): Aktivita na fóru +1 b.|
Vaše hodnocení části 3): 2 bodů
  Komentář hodnocení části 3): LLT, TS, PSA, typ. + strukt., LL, SA, KA

Pokud jste obdrželi výsledek částí 1) + 2) mimo interval <0,15+5> bodů, tak
bude oříznut, tak že získáte za implementaci alespoň 0 a ne více jak 15+5 bodů.

Procentuální hodnocení modulů překladače: 
Lexikální analýza (detekce chyb): 100% (200/200)
Syntaktická analýza (detekce chyb): 97% (194/200)
Sémantická analýza (detekce chyb): 80% (111/138)
Běhové chyby (detekce): 53% (33/62)
Interpretace přeloženého kódu: 75% (721/951)
BOOLOP 0% (0/105)
BASE 0% (0/50)
CYCLES 0% (0/100)
DEFINED 0% (0/50)
FUNEXP 0% (0/150)
IFTHEN 0% (0/100)
Celkem bez rozšíření: 81% (1259/1551)