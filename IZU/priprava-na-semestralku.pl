%% index nejvetsiho prvku linearniho seznamu
	maximum([], false).
	maximum([P], P).
	maximum([H1,H2|T], P) :- H1 >= H2, maximum([H1|T], P).
	maximum([H1,H2|T], P) :- H1 < H2, maximum([H2|T], P).

	indexof([H|_], H, 0).
	indexof([_|T], X, I) :- indexof(T, X, J), I is J + 1.
	index_max_cisla([H|T], I) :- maximum([H|T], M), indexof([H|T], M, I).

%%převést linární seznam na množinu

	je_clen(X, [X|_]) :- !.
	je_clen(X, [_|T]) :- je_clen(X, T).

	seznam_do_mnoziny([], []).
	seznam_do_mnoziny([H|T], Y) :- je_clen(H, T), seznam_do_mnoziny(T, Y).
	seznam_do_mnoziny([H|T], [H|Y]) :- seznam_do_mnoziny(T, Y).

%% AtomicMax nevim jestli může být takto...
	pocetAtom([], 0).
	pocetAtom([X|XS], S) :- atomic(X), pocetAtom(XS, Sje_S), S is SS + 1;
                  pocetAtom(XS, S1), pocetAtom(XS, S2), S is S1 + S2.

%% největší dva prvky v seznamu sečíst s tím že seznam má minimálně dva prvky

	dva_nej([X1,X2], P) :- P is X1 + X2, !.
	dva_nej([H1,H2,H3|T], P) :- H1 =< H2, H1 =< H3, dva_nej([H2,H3|T], P).
	dva_nej([H1,H2,H3|T], P) :- H2 =< H1, H2 =< H3, dva_nej([H1,H3|T], P).
	dva_nej([H1,H2,H3|T], P) :- H3 =< H1, H3 =< H2, dva_nej([H1,H2|T], P).

%% Sečíst poslední a první v seznamu
	last([X], X).
	last([_|T], X) :- last(T, X).

	sumTwo([H|T], S) :- last([H|T], L), S is L + H, !.


%% sjednoceni mnoziny
	sjednoceni2([], L, L).
	sjednoceni2([H|T], L, X) :- je_clen(H, L), sjednoceni2(T, L, X), !.
	sjednoceni2([H|T], L, [H|X]) :- sjednoceni2(T, L, X).

%% sjednocení a průnik množiny

	prunik2([], _, []).
	prunik2([H|L1], L2, [H|XTAIL]) :- je_clen(H, L2), prunik2(L1, L2, XTAIL).
	prunik2([_|L1], L2, X) :- prunik2(L1, L2, X), !.

	vlozZ(P, S, S) :- je_clen(P, S), !.
	vlozZ(P, S, [P|S]).
	
	sjednoceni2([], L, L).
	sjednoceni2([H|T], L, M) :- sjednoceni2(T, L, M1), vlozZ(H, M1, M).
	prunik_se_sjednocenyma(I, J, L, M) :- sjednoceni2(I, J, K), prunik2(K, L, M).

%% sečíst největší a nejmenší v seznamu

	maximum([], false).
	maximum([P], P).
	maximum([H1,H2|T], P) :- H1 >= H2, maximum([H1|T], P).
	maximum([H1,H2|T], P) :- H1 < H2, maximum([H2|T], P).

	minimum([], false).
	minimum([P], P).
	minimum([H1,H2|T], P) :- H1 =< H2, minimum([H1|T], P).
	minimum([H1,H2|T], P) :- H1 > H2, minimum([H2|T], P).

	soucetMinMax([], 0).
	soucetMinMax(S, MinMax) :-
	minimum(S, Min),
	maximum(S, Max),
	MinMax is Min + Max.

%% Smazat posledni 2 prvky v seznamu
	removelast([_,_], []).
	removelast([_], []).
	removelast([], []).
	removelast([H|T1], [H|T2]) :- removelast(T1, T2).