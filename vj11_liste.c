#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
  2. (Zadatak slican 3. zadatku s kolokvija 17/18)
     Mozete koristiti string.h i math.h.

     Dana je struktura:
*/
typedef struct _grad {
  char ime[51];
  double x, y;
  int profit; /* pretpostavljamo da je ovo nenegativan broj */
  struct _grad* sljed;
} grad;
/*  koja opisuje isplativost posjete grada (profit >= 0). Gradovi se čuvaju u listi (stoga postoji sljed).

  a) Definirajte funkciju
       grad* umetni(grad *prvi, grad *novi)
     koja radi sljedece:
      - ako je lista (prvi) duljine 0 ili 1, grad (novi) se umece na kraj te liste
      - inace: traze se uzastopni elementi liste a i b takvi da je izraz
         z(a, novi) + z(novi, b) maksimalan, gdje je
         z(p, q) = q.profit / sqrt((p.x - q.x)^2 + (p.y - q.y)^2)
        te se novi ubacuje u listu izmedu jednog takvog para a, b
     Funkcija vraća izmijenjenu listu (prvi), tj. adresu prvog elementa.
*/

double d(grad p, grad q)
{
  return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

grad* umetni(grad *prvi, grad *novi)
{
  double max_iznos = 0;
  grad*  najbolji_a = prvi;
  grad*  a;

  novi->sljed = 0;

  /* je li lista duljine 0? */
  if (prvi == 0)
  {
    novi->sljed = 0;
    return novi;
  }

  /* nismo se vratili iz funkcije, dakle lista nije duljine 0, tj. prvi != 0; je li duljine 1? */
  if (prvi->sljed == 0)
  {
    prvi->sljed = novi;
    novi->sljed = 0;
    return prvi;
  }

  for (a = prvi; a->sljed != 0; a = a->sljed) /* prolaz do pretposlj. člana, kao (for a = 0; a < n - 1; ++a) */
  {
    double iznos = novi->profit / d(*a, *novi) +  a->sljed->profit / d(*novi, *(a->sljed));
    if (iznos > max_iznos)
    {
      max_iznos = iznos;
      najbolji_a = a;
    }
  }

  /* poredak sljedeće dvije naredbe je bitan! (zašto?) */
  novi->sljed = najbolji_a->sljed;
  najbolji_a->sljed = novi;

  return prvi;
}

/*
 b) Napišite funkciju
      grad* odvoji(grad **prvi, char *ime, double rad)
    koja u listi pronalazi grad g imena ime,
    te iz liste izbacuje sve gradove x za koje vrijedi d(g, x) <= rad.
    Funkcija vraca novu listu koja sadrži sve izbačene gradove (za ubacivanje koristite funkciju umetni).

    Uočite - "prvi" je varijabilan parametar, što znači da mu uvijek pristupamo s * ispred
    (da se radilo u pokazivaču na strukturu
     - a ne pokazivaču na pokazivač na strukturu kao ovdje -
     možda ne bi trebalo pisati *, ako dijelovima strukture pristupamo pomoću operatora ->)
*/

grad* odvoji(grad **prvi, char *ime, double rad)
{
  grad *g = 0, *e, *nova = 0; /* g - grad imena ime, e - pomocna varijabla (za iteriranje), nova - nova lista */
  for (g = *prvi; g != 0; g = g->sljed) /* ovo je alt. rjesenje bez koristenja pomocnog pokazivaca e */
    if (strcmp(g->ime, ime) == 0)
      break;

  if (g == 0)
    return 0;

  /* ako gornji if nije prosao, to znaci da je u g pronadena adresa grada s imenom ime */

  /* kao i u zadatku s prethodnih vjezbi, brisanje ima dvije faze, ovo je prva faza: */
  while (*prvi != 0 && d(**prvi, *g) <= rad) /* poredak oko && je kao i ranije bitan, uočite i nove zvjezdice zbog varijabilnog arg. */
  {
    /* umetanje ce izmijeniti (*prvi)->sljed, stoga prvo moramo zapamtiti tu vrijednost */
    grad *t = (*prvi)->sljed;

    nova = umetni(nova, *prvi);

    *prvi = t;
    /* ne dealociramo (stari) *prvi, jer sad je on dio nove liste! */
  }

  if (*prvi == 0)
    return nova;

  /* druga faza brisanja: */
  for (e = *prvi; e->sljed != 0; )
    if (d(*(e->sljed), *g) <= rad)
    {
      grad *t = e->sljed->sljed; /* opet čuvamo ovu vrijednost, jer će ju fja umetni (možda) mijenjati */

      nova = umetni(nova, e->sljed);

      e->sljed = t;
      /* ne dealociramo (stari) e->sljed, jer sad je on dio nove liste! */
    }
    else
      e = e->sljed;


  return nova;
}

/*
 3. Napišite funkciju   element* inverzija(element *prvi)   koja invertira dobivenu listu.
    Struktura element je kao u nastavku:
 */
typedef struct _element { int x; struct _element* sljed; } element;

element* inverzija(element *prvi)
{
  element *nova = 0;
  while (prvi != 0)
  {
    element *drugi = prvi->sljed;
    prvi->sljed = nova;
    nova = prvi;
    prvi = drugi;
  }
  return nova;
}

/*
  4. Napisite funkciju   element *unija(element *a, element *b)
     koja uzima dvije sortirane liste, a vraća novu sortiranu listu sastavljenu
     od kopija članova listi a i b.
     Moguce je da se elementi ponavljaju.

     (vidi malo drugačije zadatke 7.1.30 - 7.1.33 iz skripte)
*/

/* verzija slicna pogresnoj verziji sa sata, razlike:
    - uvjet je <=,
    - na kraju umjesto 'nova' vracamo inverzija(nova) */
element *unija_1(element *a, element *b)
{
  element *nova = 0;
  while (a != 0 || b != 0) /* dok barem jedna lista ima članova koje nismo kopirali */
  {
    while (a != 0 && (b == 0  ||  b != 0 && a->x <= b->x)) /* dok treba kopirati clanove iz a */
    {
      element *novi = (element *)malloc(1 * sizeof(element));
      novi->x = a->x;
      novi->sljed = nova;
      nova = novi;
      a = a->sljed;
    }
    /* donja while petlja je simetrična gornjoj; sve pojave 'a' su sada 'b' i obratno */
    while (b != 0 && (a == 0  ||  a != 0 && b->x <= a->x)) /* dok treba kopirati clanove iz a */
    {
      element *novi = (element *)malloc(1 * sizeof(element));
      novi->x = b->x;
      novi->sljed = nova;
      nova = novi;
      b = b->sljed;
    }
  }
  return inverzija(nova);
}

/* alt. verzija u kojoj dodajemo elemente na kraj liste */
element *unija_2(element *a, element *b)
{
  element *nova = 0,
          *nova_poslj = 0;
  while (a != 0 || b != 0) /* dok barem jedna lista ima članova koje nismo kopirali */
  {
    while (a != 0 && (b == 0  ||  b != 0 && a->x <= b->x)) /* dok treba kopirati clanove iz a */
    {
      element *novi = (element *)malloc(1 * sizeof(element));
      novi->x = a->x;
      novi->sljed = 0;
      if (nova == 0)
        nova = nova_poslj = novi;
      else
        nova_poslj = nova_poslj->sljed = novi; /* a = b = c  u C-u znaci:  b = c; a = b;  */
      a = a->sljed;
    }
    /* donja while petlja je simetrična gornjoj; sve pojave 'a' su sada 'b' i obratno */
    while (b != 0 && (a == 0  ||  a != 0 && b->x <= a->x)) /* dok treba kopirati clanove iz a */
    {
      element *novi = (element *)malloc(1 * sizeof(element));
      novi->x = b->x;
      novi->sljed = 0;
      if (nova == 0)
        nova = nova_poslj = novi;
      else
        nova_poslj = nova_poslj->sljed = novi;
      b = b->sljed;
    }
  }
  return nova;
}


int main(void)
{
  return 0;
}
