#include <stdlib.h>
#include <string.h>
#include "Chaine.h"
#include "SVGwriter.h"

int main(int argc,char**argv){
  char *nom_fichier;
  char *nom_fichier2;
  FILE *fic;
  FILE *fic2;

  if(argc!=3){
      printf("Erreur format: %s <NomFichier.cha> <NomFichier.txt>\n", argv[0]);
      return 1;
	}

  //Lecture de chaine
  nom_fichier=strdup(argv[1]);
  fic = fopen(nom_fichier,"r");
  Chaines *chaine=lectureChaine(fic);
  afficheChaines(chaine);

  //Ecriture
  nom_fichier2=strdup(argv[2]);
  fic2 = fopen(nom_fichier2,"w");
  ecrireChaine(chaine,fic2);

  afficheChainesSVG(chaine,"graphe");
  detruire_Chaines(chaine);
  free(nom_fichier);
  free(nom_fichier2);
  fclose(fic);
  fclose(fic2);

  return 0;
  }
