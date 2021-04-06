#include "Reseau.h"
#include "Chaine.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SVGwriter.h"


Noeud* creer_noeud(double x, double y, int num){
    Noeud* noeud = (Noeud*)malloc(sizeof(Noeud));
    noeud -> x = x;
    noeud -> y = y;
    noeud -> num = num;
    noeud -> voisins = NULL;
    return noeud;
}

CellNoeud* creer_cell_noeud(Noeud* noeud){
    CellNoeud* cell_noeud = (CellNoeud*)malloc(sizeof(CellNoeud));
    cell_noeud -> nd = noeud;
    cell_noeud ->suiv = NULL;
    return cell_noeud;
}


void detruire_Noeud(Noeud *n){
  CellNoeud *tmp;
  while (n->voisins){
    tmp = n->voisins;
    n->voisins = n->voisins->suiv;
    free(tmp);
  }
  free(n);
}

//détruit les noeuds et ses voisins
void detruire_cellNoeud(CellNoeud *cellnoeud){
	CellNoeud *tmp;
	while (cellnoeud){
		tmp = cellnoeud;
		cellnoeud = cellnoeud->suiv;
		detruire_Noeud(tmp->nd);
		free(tmp);
	}
}



//ajout du noeud au réseau
void ajout_NoeudReseau(Reseau *R, CellNoeud *cellNoeud){
	/* ajout du cellNoeud en tete de liste */
	cellNoeud -> suiv = R -> noeuds;
	R -> noeuds = cellNoeud;
    R->nbNoeuds = R->nbNoeuds+1;

}

//Renvoie un noeud et l'ajoute au réseau s'il n'est pas présent , sinon il renvoie juste le noeud
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    CellNoeud *cell_noeud = R->noeuds;
    Noeud* noeud;

	/* Parcours de la liste pour savoir si le point existe */
    while(cell_noeud){
        noeud = cell_noeud -> nd;
        if(noeud -> x == x && noeud->y == y){
            //Si le noeud a été trouvé on le retourne
            return noeud;
        }
        cell_noeud = cell_noeud->suiv;
    }

	/* le noeud n'a pas ete trouve */

    /* creation du noeud */
    noeud = creer_noeud(x,y,R->nbNoeuds+1);

    /* creation du cell_noeud */
    cell_noeud = creer_cell_noeud(noeud);

    /*ajout du noeud dans le réseau */
    ajout_NoeudReseau(R,cell_noeud);
    return noeud;
}

CellCommodite* creer_commodite(Noeud* A, Noeud* B){
    CellCommodite* cell_commodite = (CellCommodite*)malloc(sizeof(CellCommodite));
    cell_commodite -> extrA = A;
    cell_commodite -> extrB = B;
    cell_commodite -> suiv = NULL;
    return cell_commodite;
}

void detruire_CellCommodite(CellCommodite *cellcomm){
  CellCommodite *tmp;
  while(cellcomm){
    tmp = cellcomm;
    cellcomm = cellcomm->suiv;
    free(tmp);
  }
}

Reseau* creer_reseau(int gamma){
    Reseau* reseau = (Reseau*)malloc(sizeof(Reseau));
    reseau -> nbNoeuds = 0;
    reseau -> gamma = gamma;
    reseau -> noeuds = NULL;
    reseau -> commodites = NULL;
    return reseau;
}

void detruire_reseau(Reseau *r){
	detruire_cellNoeud(r->noeuds);
	detruire_CellCommodite(r->commodites);
	free(r);
}

//Ajout des commodités au réseau
int ajouter_voisin(Noeud* A, Noeud* B){
    CellNoeud* voisinsA = A->voisins;
    CellNoeud* voisinB = creer_cell_noeud(B);

    //Ajout des commodités au réseau
    while(voisinsA){
        if(B->num == (voisinsA->nd) -> num){
            return 0;
        }
        voisinsA = voisinsA -> suiv;
    }

    //Ajout des commodités au réseau
    voisinB->suiv = voisinsA;
    A->voisins = voisinB;
    return 1;
}

//Ajout des commodités au réseau
void ajouter_commodite(Reseau *R,CellCommodite *commodite){
    commodite -> suiv = R -> commodites;
    R->commodites = commodite;
} 

Reseau* reconstitueReseauListe(Chaines *C){
    Noeud* extremiteA;
    Noeud* extremiteB;
    CellPoint* cell_point;
    CellPoint* cell_point_tmp;
	Noeud *prec; 
    Noeud* noeud;

    //création du réseau
    Reseau* R = creer_reseau(C->gamma);
    CellChaine* cell_chaine = C -> chaines;

	/* on parcourt chaque point de chaque chaine de C et on l'ajoute a la liste de noeuds de R s'il n'est pas present */
    while(cell_chaine){

        cell_point = cell_chaine -> points;
        prec = NULL;

        if(cell_point){
            extremiteA = rechercheCreeNoeudListe(R,cell_point ->x,cell_point->y);
        }

        while(cell_point){

			/* on ajoute le noeud si on ne l'a pas deja rencontre grâce à la fonction rechercheCreeNoeudListe,
            et on renvoie le noeud pour ajout_CellNoeudVoisin
			noeudCourant correspondant à un noeud contenant un point */

            noeud = rechercheCreeNoeudListe(R,cell_point ->x,cell_point->y);


			// on ajoute ses voisins

            //son voisin précédent    
			if (prec){
				ajouter_voisin(noeud, prec);
			}

			//son voisin suivant
            if (cell_point -> suiv){
				ajouter_voisin(noeud, rechercheCreeNoeudListe(R, cell_point-> suiv -> x, cell_point -> suiv -> y));
			}

            prec = noeud;
            cell_point = cell_point -> suiv;
        }

		/* la chaine compte plus d'un point , cela veut dire qu'on peut avoir des commodités */
		if (comptePointsTotal(C) > 1){
			extremiteB = prec;
			ajouter_commodite(R, creer_commodite(extremiteA, extremiteB));
		}

		cell_chaine = cell_chaine -> suiv;
    }
    return R;
}

int nbCommodites(Reseau *R){
    int nb_commodites = 0;
    CellCommodite *commodites = R -> commodites;
    while(commodites!=NULL){
        commodites = commodites -> suiv;
        nb_commodites++;
    }
    return nb_commodites;
}

int nbLiaison(Reseau *R){
	int nbTotalLiaisons; /* nombre total de liaisons */
	CellNoeud *cell_noeudcourant; /* cellule noeud courant */
	Noeud *noeudCourant;  /* noeud courant */
	CellNoeud *voisin; /* voisin du noeud courant */

	cell_noeudcourant = R -> noeuds;
	nbTotalLiaisons = 0;

	/* parcours des CellNoeud */
	while (cell_noeudcourant){
		noeudCourant = cell_noeudcourant -> nd;
		voisin =noeudCourant-> voisins;
		/* on parcourt les voisins de noeudCourant */
		while (voisin){
			/* si on n'est pas encore passé sur voisin, on compte une liaison en +
			   on sait qu'on est deja passe sur voisin si le numero de son noeud est supérieur à celui du noeud_courant*/
			if (voisin -> nd -> num < noeudCourant -> num){
				nbTotalLiaisons++;
			}
			voisin = voisin -> suiv;
		}
    cell_noeudcourant = cell_noeudcourant -> suiv;
	}
	return nbTotalLiaisons;
}


void ecrireReseau(Reseau *R, FILE *f){

    fprintf(f,"NbNoeuds :%d\n",R->nbNoeuds);
    fprintf(f,"NbLiaisons:%d\n",nbLiaison(R));
    fprintf(f,"Nbcommodites:%d\n",nbCommodites(R));
    fprintf(f,"gamma :%d\n\n",R->gamma);

	Noeud *noeud;
	CellNoeud *voisin;
    CellNoeud* cell_noeud = R -> noeuds;
    while(cell_noeud){
        fprintf(f,"v %d %f %f\n",cell_noeud->nd->num,cell_noeud->nd->x,cell_noeud->nd->y);
        cell_noeud = cell_noeud -> suiv;
    }

    fputs("\n",f);

    cell_noeud = R -> noeuds;

    while (cell_noeud){
		noeud = cell_noeud -> nd;
		voisin = noeud -> voisins;

    /* parcours des voisins */
		while (voisin){
			/* on sait qu'on est deja passe sur voisin si le numero de son noeud est supérieur à celui du noeud*/
			if (voisin -> nd -> num < noeud -> num){
				fprintf(f, "l %d %d\n", voisin -> nd -> num, noeud -> num);
			}
			voisin = voisin -> suiv;
		}
		cell_noeud = cell_noeud -> suiv;
	}

    fputs("\n",f);

    CellCommodite *cell_commodite = R->commodites;
    while(cell_commodite){
        fprintf(f,"k %d %d\n",cell_commodite->extrA->num,cell_commodite->extrB->num);
        cell_commodite = cell_commodite-> suiv;

    }

    fputs("\n",f);
}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}
