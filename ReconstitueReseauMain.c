#include <stdlib.h>
#include <string.h>
#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"


int main(int argc,char**argv){

    if(argc!=3){
        printf("Erreur format: %s <NomFichier.cha> <Numero_methode>\n", argv[0]);
        return 0;
    }


    FILE *fic; 
    FILE *fic2;
    char *nom_fichier;
    int choix;
    Chaines *instance;
    Reseau *R;


    choix=atoi(argv[2]);
    nom_fichier=strdup(argv[1]);
    fic = fopen(nom_fichier,"r");
    if(!fic){
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return 1;
    }

    instance=lectureChaine(fic);

    switch(choix){
        case 1:
            printf("Reconstitution avec liste chainee\n");
            R = reconstitueReseauListe(instance);
            fic2=fopen("test_reconstitue_reseau.txt","w");
            afficheReseauSVG(R,"graphe2");
            if(!fic2){
                printf("Erreur\n");
                return 1;
            }
            ecrireReseau(R,fic2);
            printf("Reseau de l'instance %s reconstitue avec succes\n", nom_fichier); 
            break;
    }

    detruire_Chaines(instance);
    detruire_reseau(R);
    free(nom_fichier);
    fclose(fic2);
    fclose(fic);

    return 0;
}
