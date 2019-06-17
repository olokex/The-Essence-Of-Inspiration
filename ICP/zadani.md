# ICP zadání 2018/2019
Zadání projektu


Základní zadání (je téměř shodné s předmětem IJA)

Zadání: Navrhněte a implementujte aplikaci pro přehrávání šachových partií.

Poznámka: Zadání definuje podstatné vlastnosti aplikace, které musí být splněny. Předpokládá se, že detaily řešení si doplní řešitelské týmy. Nejasnosti v zadání řešte, prosím, primárně na k tomu příslušném Fóru.

Týmy budou dvoučlenné, výjimečně může být tým i tříčlenný a bude řešit rozšířené zadání (viz níže). Jeden člen týmu je vedoucí a ten projekt odevzdá podle pokynů uvedených níže.


Specifikace požadavků

Základní požadavky

aplikace umožňuje přehrávat šachové partie zaznamenané v této notaci (nepoužívejte ani nevymýšlejte jinou) a zaznamenávat tahy provedené uživatelem

aplikace obsahuje grafické uživatelské rozhraní

šachovnice je zobrazena včetně popisů řádků (1-8) a sloupců (a-h)

figurky jsou reprezentovány obrázky

vedle šachovnice je zobrazen zápis partie

je možné otevřít více partií ve více tabech (nikoliv oknech); tyto partie jsou nezávislé

vždy se provádí kontrola, zda jsou tahy přípustné podle pravidel (není třeba uvažovat všechny detaily, postačí základní tahy figur)

Základní funkce

po spuštění je možné načíst partii, provede se ověření formátu zápisu

přehrávání je buď manuální nebo automatizované, je možné kdykoliv přepínat mezi těmito režimy; je možné přehrávat (krokovat) partii dopředu i zpět

manuální: ovládá uživatel prostřednictvím tlačítek

automatizované: je možné nastavit prodlevu mezi jednotlivými kroky; kdykoliv je možné zastavit a pokračovat dopředu či zpět

kdykoliv je možné restartovat přehrávání na začátek partie

při přehrávání se vždy zobrazuje celý zápis partie a zvýrazňuje pozice aktuálního tahu

kliknutím na pozici v zápisu lze okamžitě zobrazit stav partie na této pozici; další krokování pokračuje z této pozice

Vlastní tahy uživatele

pokud uživatel provede vlastní tah při zastaveném přehrávání, zruší se zbytek zápisu od aktuální pozice a další vývoj partie již provádí uživatel

nové tahy uživatele se zaznamenávají do zápisu

tuto novou partii je opět možné přehrávat

uvedený postup je možné opakovat - pokud je rozehrána a krokována vlastní partie a je právě zobrazen některý ze starších tahů, může uživatel opět provést vlastní tah; v tom případě se opět zruší zbytek zápisu od zobrazeného tahu

možnost operace undo a redo: tahy provedené uživatelem lze vracet zpět (undo) a opět obnovit (redo) až do okamžiku před prvním vlastním tahem, tj. až do návratu k původní načtené partii (undo a redo při přehrávání partie nedává smysl)

nově vytvořený zápis partie lze uložit

Rozšíření pro tříčlenný tým
Rozšiřte implementaci o hru dámu. Je možné současně (v různých tabech) rozehrát šach i dámu za stejných podmínek.

Postačí základní pravidla hry dáma (základní postavení, posun kamenů, skákání, přeměna kamene na dámu). Formát zápisu hry zvolte sami.

Notace zápisu
Pro zápis tahů použijte notaci definovanou tímto dokumentem (pouze základní symboly uvedené v sekcích Šachová notace a zápis tahů a Tah, braní, šach, proměna).

Zápis může být v plné nebo zkrácené notaci (aplikace umí pracovat s oběma variantami)

příklad plné notace: 10. Jg1f3 d7d5

příklad zkrácené notace: 10. Jf3 d5

Formát zápisu v souboru

každý řádek záznamu odpovídá jednomu tahu = tah bílého následovaný tahem černého

každý tah je číslován, začíná se od 1

za číslem je vždy tečka, číslo tahu a jednotlivé půltahy (bílý a černý) jsou odděleny jednou mezerou

příklad zápisu ve zkrácené notaci:

1. e4 e5
2. Sc4 Df6
příklad zápisu v plné notaci:

1. e2e4 e7e5
2. Sf1c4 Dd8f6
Součást odevzdání
Připravte předem alespoň 3 různě dlouhé uložené partie.

Doporučení

zamyslete se nad použitím vhodných návrhových vzorů

Další požadavky
Zadání definuje podstatné vlastnosti aplikace, které musí být splněny. Předpokládá se, že detaily řešení si doplní řešitelské týmy.

Při návrhu postupujte tak, aby výsledný program byl dobře použitelný. Hodnotit se bude nejen to, zda splňuje výše uvedené požadavky, ale také JAK je splňuje.

Návrh musí oddělit vlastní model aplikace od GUI (navrhněte vhodné rozhraní).

Návrh programu zdokumentujte ve zdrojovém kódu ve formátu pro program "doxygen".

Používejte vhodné návrhové vzory (Design Patterns) - viz literatura.

Pro vytváření menu, dialogů a dalších komponent grafického uživatelského rozhraní (GUI) použijte toolkit Qt minimálně ve verzi 5.5 (nainstalováno v učebnách a na serveru "merlin" v adresáři /usr/local/share/Qt-5.5.1/). Pokud pro vývoj použijete verzi novější/starší než je na učebnách, předpokládám, že zajistíte zpětnou/dopřednou kompatibilitu vašich programů. (V případě problémů, počítejte s nutností předvedení překladu/sestavení/běhu aplikace na vašem notebooku. Toto se nedoporučuje.) 
Protože toolkity jsou přenositelné na více platforem, měly by vaše programy fungovat minimálně na Linuxu i na Windows. (Nejen Java umožňuje psaní přenositelných aplikací.) Je jedno na které platformě proběhne vývoj aplikací, jen dbejte na pravidla psaní přenositelných programů. 
(Testovat se bude na Linuxu = server merlin, ale Makefile nesmí být závislý na konkrétním adresáři s lokální instalací toolkitu.) 
Na vytvoření GUI dialogů a menu můžete použít interaktivní editory/generátory.

Použijte C++14 (nebo C++17). (Pokud použijete knihovny z Boost, tak použité soubory přibalte. Existuje nástroj BCP pro výběr podmnožiny Boost.).

Programy budou vytvořeny zadáním příkazu "make" v adresáři s rozbalenými zdrojovými kódy projektu (Pozor - bude se automaticky překládat). Musíte vytvořit soubor Makefile, který nesmí obsahovat absolutní cesty k souborům. Výsledný program se bude jmenovat "chess" . Pro testovací účely půjde vše spustit použitím "make run".

Součástí projektu bude programová dokumentace vytvořená programem Doxygen (konfigurační soubor nastavte na HTML výstup se zahrnutím zdrojových textů: SOURCE_BROWSER = YES). Dokumentace se vygeneruje automaticky (= nezabalovat do archivu) při zadání "make doxygen" a bude uložena v podadresáři doc.

Poznámky k odevzdávání
Termín odevzdání je uveden v IS.

Řešení zabalené ve formátu .zip (nebo .tar.gz) odevzdá pouze vedoucí týmu do IS a to s dostatečným předstihem před uzávěrkou. (Nikdo nebude psát, že neodevzdal, protože *** a posílat to poštou.) 
Jméno odevzdaného souboru musí mít tvar: xname01-xname02.zip, kde xname01 je jméno vedoucího.

POZOR: Archiv může mít maximálně 2MB, aby mohl být vložen do IS.

Odevzdaný archiv obsahuje minimálně tuto adresářovou strukturu:

./src/*       - zdrojové texty
./examples/*  - příklady (pro testování)
./doc/        - dokumentace (bude později generován: "make doxygen")
./README.txt  - základní přehled o co jde, autoři, poznámky, ...
./Makefile    - zavolá rekurzivně make na src/Makefile, případně
                na generování dokumentace atd
Neodevzdávejte nic, co lze vytvořit pomocí "make" (spustitelné programy ani dokumentaci, kterou lze vygenerovat použitím "make doxygen"). 
Poznámka: Příkaz "make clean" všechny produkty překladu smaže, "make pack" vytvoří archiv pro odevzdání - zkontrolujte, zda obsahuje vše potřebné rozbalením v prázdném adresáři a překladem/spuštěním.

Všechny zdrojové soubory musí obsahovat na začátku komentář se jmény autorů daného modulu, popisem obsahu modulu, atd. Z komentářů by také mělo být patrné, které části byly napsány ručně a které vygenerovány automaticky.

Poznámky k hodnocení
Projekt je hodnocen na stupnici 0-100 procent/bodů.

Po ohodnocení může dojít k přepočtu bodů na jednotlivé členy týmu podle návrhu vedoucího týmu, který je zakódován do jména odevzdaného souboru (formát jména pak je: xname01-xname02-AA-BB.zip součet AA+BB=100, POZOR: pro rovnoměrné rozdělení AA=BB=50% není třeba uvádět "-50-50" a je to preferovaná možnost).

Hodnocení zahrnuje kvalitu OO návrhu, kvalitu implementace v C++, dodržování stylu psaní programu (odsazování, kvalita komentářů, vhodné identifikátory) a především funkčnost programu. Pokud budou ve zdrojových textech chybět podstatné komentáře (zvláště jména autorů), bude projekt hodnocen 0b.

POZOR: 
Pokud nepůjde přeložit příkazem make = 0b nebo předvedete na svém notebooku a vysvětlíte kde byl problém, 
pokud nebudou komentáře se jmény autorů = 0b. 
Zcela nevhodně formátovaný a nečitelný kód = 0b. 
Plagiáty atd. = 0b + návštěva u disciplinární komise.