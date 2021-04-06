#include "Chaine.h"
#include "SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NBMAX 255

CellPoint *creer_CellPoint(double x, double y){
  CellPoint *cellPoint = (CellPoint *)malloc(sizeof(CellPoint));
  if (!cellPoint) return NULL;
  cellPoint->x = x;
  cellPoint->y = y;
  cellPoint->suiv = NULL;
  return cellPoint;
}

CellChaine *creer_CellChaine(int numero){
  CellChaine *cellChaine = (CellChaine *)malloc(sizeof(CellChaine));
  if (!cellChaine) return NULL;
  cellChaine->numero = numero;
  cellChaine->points = NULL;
  cellChaine->suiv = NULL;
  return cellChaine;
}

Chaines *creer_Chaines(int nbChaines, int gamma){
  Chaines *chaine=(Chaines *)malloc(sizeof(Chaines));
  if (!chaine) return NULL;
  chaine->gamma=gamma;
  chaine->nbChaines=nbChaines;
  chaine->chaines=NULL;
  return chaine;
}


void detruire_CellPoint(CellPoint* celpoint){
	if(!celpoint) return;
	CellPoint* tmp;
	while(celpoint){
		tmp = celpoint->suiv;
		free(celpoint);
		celpoint = tmp;
	}
}

void detruire_CellChaine(CellChaine* celchaine){
	if(!celchaine) return;
	CellChaine* tmp;
	while(celchaine) {
		tmp=celchaine->suiv;
		detruire_CellPoint(celchaine->points);
		free(celchaine);
		celchaine = tmp;
	}
}

void detruire_Chaines(Chaines* chaine){
  if(!chaine) return;
  detruire_CellChaine(chaine->chaines);
	free(chaine);
}


void ajout_en_tete_CellPoint(CellChaine *cel_chaine, CellPoint *new_point){
  // Le premier point de la liste cel_chaine devient deuxième
	new_point -> suiv = cel_chaine -> points;
  // Le nouveau point devient premier
	cel_chaine -> points = new_point;
}

void ajout_en_tete_CellChaine(Chaines *chaine,  CellChaine *new_cel_chaine){
	new_cel_chaine -> suiv = chaine -> chaines;
	chaine->chaines = new_cel_chaine;
}

void afficheChaines(Chaines *chaine){
	CellChaine *cel_chaine;
	CellPoint *cel_point;
	printf("Gamma = %d\n", chaine -> gamma);
	printf("Nombre de chaines = %d\n", chaine -> nbChaines);

	/* premiere cellule de la liste chainee de "chaine" */
	cel_chaine = chaine -> chaines;

	while (cel_chaine){
		printf("\tChaine numero %d\n", cel_chaine -> numero);
		/* premier point de la liste chainee "cel_chaine" */
		cel_point = cel_chaine -> points;

		while (cel_point){
			printf("\t\t x = %.2lf y = %.2lf\n", cel_point -> x, cel_point -> y);
			cel_point = cel_point -> suiv;
		}

		cel_chaine = cel_chaine -> suiv;
	}

}

Chaines* lectureChaine(FILE *f){
	char buffer[NBMAX];
    char tmp[NBMAX];
	int nbChaines;
	int gamma;
    int numero;
	int nbPoints;
	CellChaine *cel_chaine;
	Chaines *chaine;
    CellPoint *cel_point;
	double x;
	double y;

    //Initialise de nbChaines
	fgets(buffer, NBMAX, f);
    sscanf(buffer,"%s %d",tmp,&nbChaines);

    //Initialisation de Gamme
    fgets(buffer, NBMAX, f);
    sscanf(buffer,"%s %d",tmp,&gamma);

    //Creation de chaine
    chaine=creer_Chaines(nbChaines,gamma);

    for(int i=0;i<chaine->nbChaines;i++){
        fscanf(f,"%d %d",&numero,&nbPoints);
        cel_chaine=creer_CellChaine(numero);
        for(int j=0;j<nbPoints;j++){
            fscanf(f,"%lf %lf",&x,&y);
            cel_point=creer_CellPoint(x,y);
            ajout_en_tete_CellPoint(cel_chaine,cel_point);
        }
        ajout_en_tete_CellChaine(chaine,cel_chaine);
    }
    return chaine;
}

int comptePoints(CellChaine *cel_chaine){
    CellPoint *cel_point;
    cel_point=cel_chaine->points;
    int nb=0;
    while(cel_point){
        nb++;
        cel_point=cel_point->suiv;
    }
    return nb;
}

void ecrireChaine(Chaines *C, FILE *f){
	CellChaine *cel_chaine;
	CellPoint *cel_point;

	fprintf(f,"NbChain: %d\n",C->nbChaines);
	fprintf(f,"Gamma: %d\n",C->gamma);
	fprintf(f,"\n");

	cel_chaine=C->chaines;

	while(cel_chaine){

		fprintf(f,"%d ", cel_chaine->numero);
		fprintf(f,"%d ", comptePoints(cel_chaine));
		cel_point=cel_chaine->points;

        while(cel_point){
		    fprintf(f,"%.2f %.2f ",cel_point->x,cel_point->y);
		    cel_point=cel_point->suiv;
		}
		fprintf(f,"\n");
        cel_chaine=cel_chaine->suiv;
	}
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
        precx=pcour->x;
        precy=pcour->y;
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}


double longueurChaine(CellChaine *C){
	double sommeLongueurs=0;
	double xA, yA;
	double xB, yB;
	CellPoint *cel_point = C -> points;

	while (cel_point){
		if (cel_point -> suiv){
			xA = cel_point -> x;
			yA = cel_point -> y;
			xB = cel_point -> suiv -> x;
			yB = cel_point -> suiv -> y;
			sommeLongueurs += sqrt((xB - xA)*(xB - xA) + (yB - yA)*(yB - yA));
		}
		cel_point = cel_point -> suiv;
	}
	return sommeLongueurs;
}


double longueurTotale(Chaines *C){
	double total=0;
	CellChaine *cel_chaine;

	cel_chaine = C -> chaines;
	while (cel_chaine){
		total += longueurChaine(cel_chaine);
		cel_chaine = cel_chaine -> suiv;
  }
  return total;
}

int comptePointsTotal(Chaines *C){
	CellChaine *cel_chaine;
	CellPoint *cel_point;
	int nbPoints=0;

    cel_chaine = C -> chaines;
	while (cel_chaine){
		nbPoints += comptePoints(cel_chaine);
		cel_chaine = cel_chaine -> suiv;
	}
	return nbPoints;
}
