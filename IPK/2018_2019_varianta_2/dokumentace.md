# Klient pro OpenWeatherMap API
### Popis problému
Bylo potřeba udělat klienta, který by komunikoval s OpenWeatherMap serverem, pomocí jejich API. Tak abychom mohli pomocí _make_ dostávat údaje o počasí v konkrétním městě.

### Jak vzniklý problém vyřešit?
Zvolením vhodného programovacího jazyka, který nám ulehčí co nejvíce práce. Je vysoce žádoucí se zaregistrovat [zde](https://openweathermap.org/). Pro získání API klíče, s kterým se bude spouštět daný klient (každý klíč má omezení na 60 požadavků za minutu).

- Získání _korektních_ argumentů
- Vytvoření socketu
- Navázání spojení
- Zaslat požadavek
- Zpracovat data
- Uzavřít spojení
- _Vyseparovat_ informace, které jsou pro nás podstatné
	- Případně dát vědět uživateli o vzniklé chybě
	- Zpracovat vrácené XML, HTML, ...
- Vytisknout dané informace

### Instalace a překlad
Instalace ani překlad nejsou nikterak potřeba. Aplikace je napsána v pythonu 3.6, tudíž stačí mít jen interpret nainstalovaný (linux velmi často má již interpret zavedený).

### Spuštění
Po rozbalení, stačí v daném adresáři napsat:
```sh
make run api_key=Klíč city="Požadované město"
```

Pro **api_key** je potřeba API klíč z [OpenWeather](https://home.openweathermap.org/api_keys), pro **city** je požadované město, pokud město má víceslovný název je nutno použít dvojité uvozovky. Názvy měst jsou tzv. case-insensitive (nezáleží na velikosti písmen).

#### Příklad použití  
```sh
make run api_key=b885709f0264bdcf028c5c9d040ca927 city=Brno
```

>City: Brno, CZ  
Weather: Clouds  
Temperature: 3.32 °C  
Humidity: 74%  
Pressure: 1029 hPa  
Wind speed: 25.92 km/h  
Wind degree: 40  

#### Pro víceslovné názvy měst

```sh
make run api_key=b885709f0264bdcf028c5c9d040ca927 city="Novy Hrozenkov"
```

>City: Novy Hrozenkov, CZ  
Weather: Clouds  
Temperature: 0.70 °C  
Humidity: 69%  
Pressure: 1027 hPa  
Wind speed: 25.92 km/h  
Wind degree: 60  

### Jak by se dal klient rozšířit?
- Mít možnost zvolit jednotku pro teplotu (Celsius, Fahrenheit, Kelvin)
- Pro _wind degree_ by bylo vhodnější slovní směr (sever, jih, západ, východ a jejich kombinace)
- Možnost zadat město podle koordinátů