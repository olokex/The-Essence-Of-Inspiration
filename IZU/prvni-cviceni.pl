muz(rickard).
muz(brandon). 
muz(eddard).
muz(benjen).
muz(jon). 
muz(robb).
muz(tyrion).
muz(bran). 
muz(rickon).

zena(xxx). 
zena(wylla). 
zena(catelyn).
zena(lyanna). 
zena(talisa). 
zena(sansa).
zena(arya).

otec(rickard,brandon). 
otec(rickard,eddard). 
otec(rickard,benjen). 
otec(rickard,lyanna). 
otec(eddard,jon). 
otec(eddard,robb).
otec(eddard,sansa).
otec(eddard,arya).
otec(eddard,bran).
otec(eddard,rickon). 


matka(xxx,brandon). 
matka(xxx,eddard).
matka(xxx,benjen).
matka(xxx,lyanna).
matka(wylla,jon).
matka(catelyn,robb).
matka(catelyn,sansa).
matka(catelyn,arya).
matka(catelyn,bran).
matka(catelyn,rickon).    


rodic(X,Y) :- otec(X,Y).
rodic(X,Y) :- matka(X,Y).

je_matka(X) :- matka(X,_).



%tato pravidla doplnte
%sourozenec(X,Y) :- 
%stryc(X,Y) :-
%bratr(X, Y) :- 
%babicka(X, Y) :-


%oba sourozenci jsou vlastni
%jinak by stacila pouze jedna strana
sourozenec(X,Y) :- otec(O,X), otec(O,Y), matka(M,X), matka(M,Y).
stryc(X,Y) :- muz(X), rodic(R,Y), sourozenec(X, R).
bratr(X, Y) :- muz(X), sourozenec(Y, X).
babicka(X, Y) :- zena(X), rodic(R, Y), rodic(X,R).