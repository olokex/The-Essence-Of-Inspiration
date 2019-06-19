```Komentář učitele k hodnocení:
PŘÍKLAD  MAX          VAŠE BODY
1              0.5           0.5           
2              1              1              
3              0.5           0.5           OK, jen většinou se spíš póly kreslí jako křížky a nuly jako kolečka
4              0.5           0.5           
5              1              1              
6              1              1              
7              1              1              
8              1              1              
9              1              0.5           Tvar je ok, ale histogram byste měl vynormalizovat počtem vzorků a velikostí jednoho binu. Když pak počítáte integrál, měl byste hodnoty napřed vynásobit velikostí binu a pak teprve sečíst (počítáte obsah pod funkcí, obsah každého sloupce histogramu můžete spočítat jako šířka*výška, kde šířka je velikost binu a výška hodnota p(x)). I když od oka vypočítáte integrál funkce, kterou máte na obrázku, tak nevyjde 1 - levá část je široká asi 0.5a vysoká asi 0.03, tzn. má obsah asi 0.015, podobně pravá část.
10             0.5           0.5           numpy.correlate se dá použít (se správnou normalizací), ale takhle to ručně spočítat je taky možnost
11             0.5           0.5           
12             1.5           1.5           Není mi úplně jasné, proč násobíte signál *(-1) na vstupu histogram2d. Pak při vykreslení to spravíte tím, jak tam nastavujete extent, ale vypočítané to není úplně správně. Jestli jste se snažil jen překlopit y-osu, tak můžete pro imshow použít origin='low'.
13             1              1              Tady počítáte ten integrál správně! Násobíte každou hodnotu `binsize`, pak sčítáte. Podobně to mělo být v Př.9.
14             1              1              Zkuste použít np.outer(x1,x2) místo x1*x2 při počítání té korelace. Tím vytvoříte matici, která bude mít na každém políčku součin 'souřadnic' toho políčka. Tak, jak to děláte vy, vytváříte vektor, který je součinem x1 a x2, který se při násobení s maticí bins nakopíruje tak, aby měl stejný tvar jako ta matice, což není výsledek, který chcete.
               
TOTAL                    11.5      pěkné!
```