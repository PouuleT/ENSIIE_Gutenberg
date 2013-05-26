#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    FILE* matrice = fopen(argv[1],"r");
    analyze_matrix(matrice);
    return 1;
}

int analyze_matrix(FILE* matrice) {
    char car[1000];

    int i=0,j=0;
    int o,p;
    int end=0;
    int k=0;
    char c;

    if(matrice == NULL) {
        printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
        printf("Fin du programme\n");
        return -1;
    }
    else {
        printf("Analyse du fichier de matrice ...\n ");
        do
        {
            c = fgetc(matrice);
            if(c == EOF) {
                end = 1;
                printf("\nFin du fichier\n");
            }
            else {
                if(c == 27)
                    printf("\^\[");
                else if(c == 1)
                    printf("[]");
                else
                {
                    printf("%c", c);
                }
                k++;
            }
        } while (!end);
        printf("\nLe fichier comporte %d lignes\n", k);
    }
    printf("Done\n");
    //    printf("The result : \n%s", car);
    return car;
}

