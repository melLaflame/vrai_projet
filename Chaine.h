#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include<stdio.h>

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                  /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

Chaines *creer_Chaines(int nbChaines, int gamma);
CellChaine *creer_CellChaine(int numero);
CellPoint *creer_CellPoint(double x, double y);
void detruire_CellPoint(CellPoint* celpoint);
void detruire_CellChaine(CellChaine* celchaine);
void detruire_Chaines(Chaines* chaine);
void ajout_en_tete_CellPoint(CellChaine *cel_chaine, CellPoint *new_point);
void ajout_en_tete_CellChaine(Chaines *chaine,  CellChaine *new_cel_chaine);
void afficheChaines(Chaines *chaine);
Chaines* lectureChaine(FILE *f);
void ecrireChaine(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);

#endif	
