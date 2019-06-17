% prvni uloha
posledny([X|[]],X) :- !.
posledny([_|K], P) :- posledny(K, P).

% druha uloha
zmaz_na(1, [Z|K], Z, K) :- !.
zmaz_na(P, [Z|L], X, [Z|Zbytek]) :- P1 is P - 1, zmaz_na(P1, L, X, Zbytek).

% treti uloha
%% pridat(1, [Z|K], X, [X, Z|K]) :- !.
%% pridat(P, [Z|L], X, [Z|Zbytek]) :- P1 is P - 1, pridat(P1, L, X, Zbytek).

pridat(1, [Z|K], X, [X, Z|K]) :- !.
pridat(P, [Z|L], X, [Z|Zbytek]) :- P1 is P - 1, zmaz_na(P1, Zbytek, X, L). 
%pridat(P, [Z|L], X, [Z|Zbytek]) :- P1 is P - 1, zmaz_na(P1, Zbytek, X, L). 