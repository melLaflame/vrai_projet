#ifndef __HACHAGE_H__
#define __HACHAGE_H__	
#include "Reseau.h"	
#include <stdio.h>

typedef struct element {
    CellNoeud * cellnoeud;
    int cle;
}Element;

typedef struct tableHachage {
    Element** tableau;
    int taillemax;
}TableHachage;

double fonction_clef(double x, double y);
int fonction_de_hachage(int M, double k);

#endif


