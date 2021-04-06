all:ChaineMain ReconstitueReseauMain

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	gcc ChaineMain.o Chaine.o SVGwriter.o -o ChaineMain

ReconstitueReseauMain: ReconstitueReseauMain.o Reseau.o Chaine.o SVGwriter.o
	gcc ReconstitueReseauMain.o Chaine.o Reseau.o SVGwriter.o -o ReconstitueReseauMain

Chaine.o: Chaine.c Chaine.h SVGwriter.h
	gcc -c Chaine.c -o Chaine.o

ReconstitueReseauMain.o: ReconstitueReseauMain.c Chaine.h Reseau.h SVGwriter.h
	gcc -c ReconstitueReseauMain.c -o ReconstitueReseauMain.o

Reseau.o: Reseau.c Reseau.h Chaine.h SVGwriter.h
	gcc -c Reseau.c -o Reseau.o

SVGwriter.o: SVGwriter.c SVGwriter.h
	gcc -c SVGwriter.c -o SVGwriter.o

ChaineMain.o: ChaineMain.c Chaine.h SVGwriter.h
	gcc -c ChaineMain.c -o ChaineMain.o


clean: rm-f*.o rm-f main