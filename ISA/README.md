# isa-tazatel

### Popis aplikace - obecně
Jedná se o vlastní implementaci WHOIS protokolu, který slouží pro dotazování a zjišťování záznamů vlastníků o dané IP adrese (případně hostname).

#### Co umí?
Aplikace podporuje IPv4, IPv6 a hostname.<br>
O dané adrese _vytáhne_ i DNS dotazy (A, AAAA, SOA, CNAME, NS)<br>
Přidán přepínač pro _verbose_, který ovliňuje, zda výpis je strohý. Tento přepínač je nepovinný, byť výchozí chování je **false**.<br>
Automaticky přejde na _referenci_ v whois záznamu. Toto je možné vidět pokud se vypíše `[Redirected to whois.example.com]`

#### Co neumí?
Neumí DNS jinak, než přes systémový resolv, tj. přepínač **-d** je "placeholder".<br>
Isa-tazatel je trochu "přísný" a vrací pouze to co, uživatel zadal, tedy pokud uživatel zadá `www.seznam.cz`, tak je dost možné, že pro tuto adresu není v databázi žádný záznam. Je tedy na uživateli, "co požaduje".<br>
V momentální fázi u DNS neumí získat PTR záznam.

### Překlad
Stačí použít přibalený Makefile.<br>
V systému musí být pcap a resolv knihovna.<br>
Příkaz `make`by měl zkompilovat daný zdrojový kód.

### Příklad použití
Plnátučná verze
`./isa-tazatel -q seznam.cz -w whois.nic.cz -v true -d 8.8.8.8`

> výpis

```[Querying whois.nic.cz]
[Response from whois.nic.cz]
=== WHOIS ===
%  (c) 2006-2019 CZ.NIC, z.s.p.o.
%
% Intended use of supplied data and information
%
% Data contained in the domain name register, as well as information
% supplied through public information services of CZ.NIC association,
% are appointed only for purposes connected with Internet network
% administration and operation, or for the purpose of legal or other
% similar proceedings, in process as regards a matter connected
% particularly with holding and using a concrete domain name.
%
% Full text available at:
% http://www.nic.cz/page/306/intended-use-of-supplied-data-and-information/
%
% See also a search service at http://www.nic.cz/whois/
%
%
% Whoisd Server Version: 3.12.0
% Timestamp: Mon Nov 18 16:38:11 2019
domain:       seznam.cz
registrant:   SB:SEZNAM-CZ-AS
admin-c:      SEZNAM-CZ-AS-TECH
nsset:        SEZNAM-NAMESERVERS
keyset:       SEZNAM-CZ-AS-ECDSA
registrar:    REG-IGNUM
status:       Sponsoring registrar change forbidden
registered:   07.10.1996 02:00:00
changed:      29.05.2019 14:05:04
expire:       29.10.2020
contact:      SB:SEZNAM-CZ-AS

org:          Seznam.cz, a.s.
name:         Seznam.cz, a.s.
address:      Radlická 3294/10
address:      Praha 5
address:      15000
address:      CZ
registrar:    REG-IGNUM
created:      10.08.2001 22:13:00
changed:      27.11.2018 10:30:01
contact:      SEZNAM-CZ-AS-TECH

org:          Seznam.cz, a.s.
name:         Vlastimil Pečínka
address:      Radlická 3294/10
address:      Praha 5
address:      150 00
address:      CZ
registrar:    REG-MOJEID
created:      27.02.2017 13:51:01
changed:      04.12.2018 15:48:58

nsset:        SEZNAM-NAMESERVERS
nserver:      ans.seznam.cz (77.75.74.80, 2a02:598:3333::3)
nserver:      ams.seznam.cz (77.75.75.230, 2a02:598:4444::4)
tech-c:       SB:SEZNAM-CZ-AS
registrar:    REG-IGNUM
created:      18.10.2007 18:01:01
changed:      11.12.2014 11:08:04
keyset:       SEZNAM-CZ-AS-ECDSA
dnskey:       257 3 13 +qiXHs6rSZgd2hCEut/9gKAbGHgNKE686hhiP6wUZqyXJKsV5Sm4mqXoM5zwxBdPl7Qi4cpKEj5pQdN1KwoAlg==
tech-c:       SB:SEZNAM-CZ-AS
registrar:    REG-IGNUM
created:      27.03.2018 17:36:55

=== DNS ===
NS: ams.seznam.cz
NS: ans.seznam.cz
A:  77.75.75.176
A:  77.75.75.172
AAAA:   2a02:598:4444:1::2
AAAA:   2a02:598:4444:1::1
SOA:    ans.seznam.cz
MX: seznam.cz.      5M IN MX    10 mx1.seznam.cz.
MX: seznam.cz.      5M IN MX    20 mx2.seznam.cz.
```

Osekanější možnost, kde se vypisují nejnutnější informace.

`./isa-tazatel -q seznam.cz -w whois.ripe.net`

> výpis

```[Querying whois.ripe.net]
[Response from whois.ripe.net]

=== WHOIS ===
Record not found

=== DNS ===
NS: ams.seznam.cz
NS: ans.seznam.cz
A:  77.75.75.176
A:  77.75.75.172
AAAA:   2a02:598:4444:1::2
AAAA:   2a02:598:4444:1::1
SOA:    ans.seznam.cz
MX: seznam.cz.      3m58s IN MX 10 mx1.seznam.cz.
MX: seznam.cz.      3m58s IN MX 20 mx2.seznam.cz.```