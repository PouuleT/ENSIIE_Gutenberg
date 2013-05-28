#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE* matrice = fopen(argv[1],"r");
//    analyze_matrix(matrice);
//    new_matrix_analyser(matrice);
    getWord(matrice, "LTPU");
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

int new_matrix_analyser(FILE* matrice) {
    char buff_word[100];
    char buff_num[20];
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
            else if(c >= 48 && c <=57) {
                buff_num[j] = c;
                buff_num[j+1] = '\0';
                j++;
                if(i != 0) {
                    printf("1 we got a string : %s\n", buff_word);
                    i=0;
                }
            }
            else if ((c >= 58 && c <= 126) || (c >=32 && c<=47 )) {
                buff_word[i] = c;
                buff_word[i+1] = '\0';
                i++;
                if(j != 0) {
                    printf("2 we got a number : %d\n", atoi(buff_num));
                    j=0;
                }
            }
            else if(c == 27) {
                printf("\^\[");
                if(j != 0) {
                    printf("3 we got a number : %d\n", atoi(buff_num));
                    j=0;
                }
                else if(i != 0) {
                    printf("4 we got a string : %s\n", buff_word);
                    i=0;
                }
            }
            else {
                if(j != 0) {
                    printf("5 we got a number : %d\n", atoi(buff_num));
                    j=0;
                }
                else if(i != 0) {
                    printf("6 we got a string : %s\n", buff_word);
                    i=0;
                }

                printf("unknown : %c - %d\n",c, c);
            }
//            getchar();
        } while (!end);
    }
    return 1;
}

int getWord(FILE* matrice, char* word)
{
   int LTPU[2];
   int i=0;
   int end; 
   char c;
   char buff_word[50];

   if(matrice == NULL) {
       printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
       printf("Fin du programme\n");
       return -1;
   }
   else {
       printf("Analyse du fichier de matrice a la recherche du LTPU...\n ");
       do
       {
           c = fgetc(matrice);
           if(c == EOF) {
               end = 1;
               printf("\nFin du fichier\n");
           }
           else if(c == 59){    // On veut pas du point virgule dans les mots
           }

           else if ((c >= 58 && c <= 126) || (c >=32 && c<=47 )) {
               buff_word[i] = c;
               buff_word[i+1] = '\0';
               i++;
           }

           else {
               if(i != 0) {
                   printf("we got a word : %s\n", buff_word);
                   if(strcmp(buff_word, word) == 0) {
                       printf("We got the good word, at %d\n", SEEK_CUR);
                       fseek(matrice, -5, SEEK_CUR);
                       c = fgetc(matrice);
                       printf("%c\n",c);
                       c = fgetc(matrice);
                       printf("%c\n",c);
                       return 1;
                    }
                   i=0;
               }
           }
       } while(!end);
   }
   return 0;
}

