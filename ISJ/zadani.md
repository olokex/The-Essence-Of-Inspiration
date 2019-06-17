# Zadání pro projekty do ISJ

### 1

Popis:
Stáhněte si soubor https://www.fit.vutbr.cz/study/courses/ISJ/private/isj_proj01_xnovak00.py a místo xnovak00 dejte do názvu svůj login.

Dopište definici regulárního výrazu word_parse tak, aby do skupin rozděloval slova z řetězce, oddělovaného mezerami, za nimiž nenásleduje konstrukce (P...), a slova, za nimiž tato konstrukce následuje (v tomto případě do další skupiny obsah závorky).
Například ve vstupu:
bee(P: insect honey) dog  cat (P:milk) ant(P) ape
jsou slova 'dog' a 'ape' prvního typu,
s 'bee' je asociováno 'insect honey'
s 'ant' nic (prázdný řetězec)
Mělo by být tedy možné (jak to dělá skript) získat trojice:
('', 'bee', 'insect honey')
('dog', '', '')
('', 'cat', 'milk')
('', 'ant', '')
('ape', '', '')
Dopište definici regulárního výrazu punct tak, aby odpovídal pozici za čárkou nebo tečkou, s výjimkou případu, kdy bezprostředně před znakem a za daným místem stojí číslice (a aby tedy ani následný test assert nevypisoval chybu).

Nic jiného v kódu neměňte (i kdyby se vám nelíbil nedostatek komentářů apod.).
Cvičné hodnocení můžete vyzkoušet v systému http://athena3.fit.vutbr.cz:8088/proj1, ale pro získání bodů za projekt musíte výsledný skript (se správným názvem) odevzdat prostřednictví WIS.

### 2

Stáhněte si soubor https://www.fit.vutbr.cz/study/courses/ISJ/private/isj_proj02_xnovak00.py a místo xnovak00 dejte do názvu svůj login.

V kódu doplňte pravé strany přiřazení vyznačených proměnných (jmen) tak, aby výsledky testů odpovídaly očekávaným hodnotám.

Kód je formálně rozdělen do funkcí, ale to je jen pro potřeby oddělení jednotlivých podúkolů.

Nic jiného v kódu neměňte (i kdyby se vám nelíbil nedostatek komentářů apod.).
Cvičné hodnocení můžete vyzkoušet v systému http://athena3.fit.vutbr.cz:8088/proj2, ale pro získání bodů za projekt musíte výsledný skript (se správným názvem) odevzdat prostřednictví WIS.

### 3

Stáhněte si soubor https://www.fit.vutbr.cz/study/courses/ISJ/private/isj_proj03_xnovak00.py a místo xnovak00 dejte do názvu svůj login.

V kódu doplňte pravé strany přiřazení tak, aby výsledky testů odpovídaly očekávaným hodnotám.

Nic jiného v kódu neměňte (i kdyby se vám nelíbil nedostatek komentářů apod.).
Cvičné hodnocení můžete vyzkoušet v systému http://athena3.fit.vutbr.cz:8088/proj3, ale pro získání bodů za projekt musíte výsledný skript (se správným názvem) odevzdat prostřednictví WIS.

### 4

Do souboru, nazvaného podle konvence isj_proj04_xnovak00.py, napište definice následujících funkcí:

can_be_a_set_member_or_frozenset(item) - vrátí vstupní položku item, pokud tato může být prvkem množiny v Pythonu, v opačném případě frozenset(item)
Můžete si zkusit:
    assert can_be_a_set_member_or_frozenset(1) == 1
    assert can_be_a_set_member_or_frozenset((1,2)) == (1,2)
    assert can_be_a_set_member_or_frozenset([1,2]) == frozenset([1,2])
V Pythonu nemůže být prvkem množiny jiná množina. Měli byste vědět proč.
    
all_subsets(lst) - na vstupu dostane seznam a pouze s použitím vestavěných funkcí (tedy bez použití "import") z něj vytvoří seznam, odpovídající množině všech podmnožin, tedy např.
    assert all_subsets(['a', 'b', 'c']) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
Návod: Množina všech podmnožin prázdného seznamu (prázdné množiny) má jeden prvek - prázdný seznam (množinu). Přidejte jej na začátku do vytvářeného výstupního seznamu.
Pro každý další prvek platí, že množina všech podmnožin obsahuje všechny podmnožiny, které daný prvek neobsahují (předchozí stav), a podmnožiny, které daný prvek obsahují (do nichž byl daný prvek přidán).
Tedy když jsem narazil ve výše uvedém příkladě na prvek 'c', přidal jsem jej nejprve k prázdnému seznamu, potom k seznamům ['a'], ['b'] a nakonec k ['a', 'b']).
Poznámka: Lze to samozřejmě dělat i jinak, ale aby vyšel assert, použijte prosím popsaný postup.

all_subsets_excl_empty - obdoba předchozího, ale při volání dostane prvky seznamu přímo jako argumenty a navíc má volitelný parametr exclude_empty, který, když není ve volání uveden, nebo je jeho hodnota True, vrátí výsledek bez prázdného seznamu. Pokud je hodnota tohoto argumentu False, je výsledek stejný jako u předchozí funkce. 
Takže např.:
    assert all_subsets_excl_empty('a', 'b', 'c') == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
    assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = True) == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
    assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = False) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]

Jako první řádek skriptu uveďte tzv. shebang (viz https://en.wikipedia.org/wiki/Shebang_%28Unix%29), který umožní spuštění skriptu na fakultních serverech, takže např.:
#!/usr/bin/env python3
Nezapomeňte na dokumentační řetězce všech funkcí.
Dále každou logickou část kódu jednotlivých funkcí okomentujte, např. checking whether the item can be a member of a set

### 5

Do souboru, nazvaného podle konvence isj_proj05_xnovak00.py, implementujte třídu Polynomial, která bude pracovat s polynomy reprezentovanými pomocí seznamů. Například 2x^3 - 3x + 1 bude  reprezentováno jako [1,-3,0,2] (seznam začíná nejnižším řádem, i když se polynomy většinou zapisují opačně).

Instance třídy bude možné vytvářet několika různými způsoby:
pol1 = Polynomial([1,-3,0,2])
pol2 = Polynomial(1,-3,0,2)
pol3 = Polynomial(x0=1,x3=2,x1=-3)

Volání funkce print() vypíše polynom v obvyklém formátu:
>>> print(pol2)
2x^3 - 3x + 1

Bude možné porovnávat vektory porovnávat:
>>> pol1 == pol2
True

Polynomy bude možné sčítat a umocňovat nezápornými celými čísly:
>>> print(Polynomial(1,-3,0,2) + Polynomial(0, 2, 1))
2x^3 + x^2 - x + 1
>>> print(Polynomial(-1, 1) ** 2)
x^2 - 2x  + 1

A budou fungovat metody derivative() - derivace a at_value() - hodnota polynomu pro zadané x - obě pouze vrací výsledek, nemění samotný polynom:
>>> print(pol1.derivative())
6x^2 - 3
>>> print(pol1.at_value(2))
11
>>> print(pol1.at_value(2,3))
35
(pokud jsou zadány 2 hodnoty, je výsledkem rozdíl mezi hodnotou at_value() druhého a prvního parametru - může sloužit pro výpočet určitého integrálu, ale ten nemá být implementován)

Maximální hodnocení bude vyžadovat, abyste:
- uvedli "shebang" jako v předchozích projektech
- důsledně používali dokumentační řetězce a komentovali kód
- nevypisovali žádné ladicí/testovací informace při běžném "import isj_proj05_xnovak00"
- do skriptu přidali následující testy a zajistili, že ./isj_proj05_xnovak00.py projde bez chyb
def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()

### 6

Do souboru, nazvaného podle konvence isj_proj06_xnovak00.py, implementujte:

- funkci first_nonrepeating, která dostane na vstup řetězec a vrátí první neopakující se znak ze vstupního řetězce:
tooth -> h, lool -> None

- funkci combine4, která dostane čtveřici 4 kladných celých čísel a očekávaný výsledek a vrátí setříděný seznam  unikátních řešení matematických hádanek s výsledkem operací +, -, *, / nad 4 čísly - viz např. http://blog.plover.com/math/17-puzzle.html
Například combine4([6,6,5,2],36) by mohlo vrátit něco jako:
['(2+5)*6-6', '(5+2)*6-6', '6*(2+5)-6', '6*(5+2)-6'],
případně jen ['(2+5)*6-6'], pokud se vám podaří odstraňovat výrazy kvůli komutativitě sčítání a násobení,
případně i delší seznam, jehož prvky se budou lišit jen uzávorkováním.

Řešení si můžete zkontrolovat v novém systému na adrese http://isj.fit.vutbr.cz:8090/proj. Případné problémy s kontrolním systémem prosím hlaste Juraji Kyseľovi - xkysel12@stud.fit.vutbr.cz - který systém vytváří. Odevzdání opět prostřednictvím WIS.

### 7

Do souboru, nazvaného podle konvence isj_proj07_xnovak00.py, implementujte:

1. dekorátor @limit_calls s parametry max_calls a error_message_tail (defaultní hodnoty 2 a 'called too often') pro obecné funkce tak, aby např.:

import math

@limit_calls(1, 'that is too much')
def pyth(a,b):
    c = math.sqrt(a**2 + b ** 2)
    return c
print(pyth(3,4))
print(pyth(6,8))
Vypsalo např:
5.0
Traceback (most recent call last):
  File "isj_proj07_xnovak01.py", line 43, in <module>
    print(pyth(6,8))
  File "isj_proj07_xnovak01.py", line 14, in wrapper
    raise TooManyCallsError(specific_error_message)
__main__.TooManyCallsError: function "pyth" - that is too much.

2. generátorovou funkci ordered_merge, která bude moci být volána s libovolným počtem iterovatelných objektů a parametrem selector, udávajícím, z kterého iterovatelného objektu má prvek na dané pozici být, a bude vybírat prvky v zadaném pořadí (první iterovatelný objekt má v selector index 0).
Například tedy:
print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11), selector = [2,3,0,1,3,1])))
vypíše [3.0, 11, 'a', 1, 22, 2]

3. třídu Log tak, aby po vrácení chyby z kódu:
 
with Log('mylog.txt') as logfile:
    logfile.logging('Test1')
    logfile.logging('Test2')
    a = 1/0
    logfile.logging('Test3')

bylo v souboru mylog.txt:
Begin
Test1
Test2
End

### 8 (4.5 bodu z 5)

S využitím modulu cProfile zjistěte, kolikrát je volána funkce lev() ve skriptu dostupném na https://www.fit.vutbr.cz/study/courses/ISJ/private/lev.py.
Výsledek uložte do souboru lev.cProfile_output.

Pomocí time zjistěte, jak dlouho běží skript dostupný na https://www.fit.vutbr.cz/study/courses/ISJ/private/nonpalindrom_words_existing_reversed.py na souboru staženém z https://www.fit.vutbr.cz/study/courses/ISJ/private/words30000.txt. Upravte skript tak, aby se daný čas co možná nejvíc zkrátil a aby bylo možné zpracovat i podstatně větší soubor stažený z https://www.fit.vutbr.cz/study/courses/ISJ/private/words.txt. Upravený skript pojmenujte ve formatu login_nonpalindrom_words_existing_reversed.py (tedy např. xnovak01_nonpalindrom_words_existing_reversed.py) a odevzdejte.
Výstup z time ./`id -u -n`_nonpalindrom_words_existing_reversed.py words.txt odevzdejte jako words.time_output.
Změřte čas běhu skriptů dostupných na adresách https://www.fit.vutbr.cz/study/courses/ISJ/private/sekv.py a https://www.fit.vutbr.cz/study/courses/ISJ/private/para.py a odevzdejte je jako sekv.time_output a para.time_output. Do souboru why.txt napište třípísmennou zkratku, která vysvětlí, proč se reálný čas běhu paralelní verze neblíží polovině běhu sekvenční verze. Soubor why.txt odevzdejte. 
Upravte skript para.py tak, aby reálný čas zpracování odpovídal zhruba polovině času běhu sekvenční verze. Upravenou verzi pojmenujte ve formatu login_para.py (tedy např. xnovak01_para.py) a odevzdejte.