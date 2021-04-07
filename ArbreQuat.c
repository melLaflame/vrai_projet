#include "ArbreQuat.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double*ymin, double* xmax, double* ymax){
    CellChaine *chaines = C->chaines;
    CellPoint *points;
    xmin* = C->chaines->points->x;
    xmax* = C->chaines->points->x;
    ymin* = C->chaines->points->y;
    ymax* = C->chaines->points->y;

    while(chaines){
        points = chaines->points;
        int X = points->x;
        int Y = points->y;
        while(points){
            if(X<xmin*){
                xmin* = X;
            }
            if(Y<ymin*){
                ymin* = Y;
            }
            if(X>xmax*){
                xmax* = X;
            }
            if(y>ymax*){
                ymax* = Y;
            }
            points = points->suiv;
        }
        chaines = chaines->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX,double coteY){
    ArbreQuat* new_ArbreQuat = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    new_ArbreQuat -> xc = xc;
    new_ArbreQuat -> yc = yc;
    new_ArbreQuat -> coteX = coteX;
    new_ArbreQuat -> coteY = coteY;
    new_ArbreQuat -> noeud = NULL;
    new_ArbreQuat -> so = NULL;
    new_ArbreQuat -> se = NULL;
    new_ArbreQuat -> no = NULL;
    new_ArbreQuat -> ne = NULL;
    return new_ArbreQuat
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat*parent){
    if(a*==NULL){
        if(parent == NULL){
            ArbreQuat* new_ArbreQuat = creerArbreQuat(0,0,0,0);
            new_ArbreQuat -> Noeud = n;
        }
        else{
            int X1 = n->x;
            int Y1 = n->y;
            int X2 = (parent*)->xc;
            int Y2 = (parent*)->yc;
            if(X1<X2 && Y1<Y2){
                ArbreQuat* new_ArbreQuat = creerArbreQuat((a*)->coteX/4,(a*)->coteY/4,(a*)->coteX/2,(a*)->coteY/2);
                new_ArbreQuat -> Noeud = n;
                (a*)->so = new_ArbreQuat;
            }
            if(X1<X2 && Y1>Y2){
                ArbreQuat* new_ArbreQuat = creerArbreQuat((a*)->coteX/4,3*(a*)->coteY/4,(a*)->coteX/2,(a*)->coteY/2);
                new_ArbreQuat -> Noeud = n;
                (parent*)->no = new_ArbreQuat;
            }

            if(X1>X2 && Y1<Y2){
                ArbreQuat* new_ArbreQuat = creerArbreQuat(3*(a*)->coteX/4,(a*)->coteY/4,(a*)->coteX/2,(a*)->coteY/2);
                new_ArbreQuat -> Noeud = n;
                (parent*)->se = new_ArbreQuat;
            }

            if(X1>X2 && Y1>Y2){
                ArbreQuat* new_ArbreQuat = creerArbreQuat(3*(a*)->coteX/4,3*(a*)->coteY/4,(a*)->coteX/2,(a*)->coteY/2);
                new_ArbreQuat -> Noeud = n;
                (parent*)->ne = new_ArbreQuat;
            }
        }
    }

    else if((*a)->noeud != NULL){
        Noeud* tmp = (*a)->noeud;
        (a*)->noeud = NULL;

        int X1 = n->x;
        int Y1 = n->y;
        int X2 = (a*)->xc;
        int Y2 = (a*)->yc;

        if(X1<X2 && Y1<Y2){
            insererNoeudArbre(n,(a*)->so,NULL);
        }
        if(X1<X2 && Y1>Y2){
            insererNoeudArbre((a*)->no,n,NULL);
        }

        if(X1>X2 && Y1<Y2){
            insererNoeudArbre((a*)->se,n,NULL);
        }

        if(X1>X2 && Y1>Y2){
            insererNoeudArbre((a*)->ne,n,NULL);
    }

    else if(((*a)->noeud == NULL)){

    }
}