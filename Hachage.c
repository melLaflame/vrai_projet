#include "Hachage.h"
#include <stdio.h>
#include <math.h>

double fonction_clef(double x, double y){
    return y + (x + y)*(x + y + 1)/2;
}

int fonction_de_hachage(int M, double k){
    double A = (sqrt(5)-1)/2;
    return (int)(M*(k*A-(int)(k*A)));
}

int main(void){
    int i,j;
    for(i=1;i<11;i++){
        for(j=1;j<11;j++){
            printf("%f\n",fonction_clef(i,j));
        }
    }
    return 0;
}


