#include <stdio.h>

int main()
{
   /* printf i scanf prvo primaju tzv. format, npr. "%d" za upis/ispis cijelog broja,
     a nakon toga:
      - scanf prima popis varijabli čiji se sadržaj ispiisuje
      - printf prima popis memorijskih mjesta na koje želimo pisati: u pravilu je memorijsko mjesto adresa neke varijable, npr. &a je adresa od a */

  int a = 4; /* decimalan broj */
  printf("a = %d\n", a); /* %c je format za char, tj. znak */
  printf("Upišite novu vrijednost za a: \n");
  scanf("%d", &a);
  printf("a = %d\n", a);


  char b; /* znak */
  printf("b = %c\n", b); /* %c je format za char, tj. znak; ovdje će se ispisati slučajna vrijednost jer nismo inicijalizirali b */
  printf("Upišite novu vrijednost za b: \n");
  scanf(" %c", &b); /* razmak ispred %c kako bi bio "progutan" ranije upisan razmak nakon a */
  printf("b = %c\n", b);

  double c; /* realan broj */
  printf("c = %lf\n", c); /* %lf je format za decimalan broj (lf kao long float); ovdje će se ispisati slučajna vrijednost jer nismo inicijalizirali c */
  printf("Upišite novu vrijednost za c: \n");
  scanf("%lf", &c);
  printf("c = %lf\n", c);


  return 0;
}
