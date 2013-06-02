#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int checkEnd(FILE* matrice)
{
    int i=0;
    char word;

    word = fgetc(matrice);          // On récupère le caractère suivant
    fseek(matrice, -1, SEEK_CUR);   // On se repositionne a la position indiquee avant de commencer au cas où c'est pas la fin
    if(word == 27)                  // Si c'est le caractère "echap", c'est qu'il n'y a plus de PU/PD, on arrête
    {
        return 1;
    }
    else                            // Sinon on continue
        return 0;

}

int main(int argc, char* argv[])
{
    FILE* matrice = fopen(argv[1],"r");
    char **ptr=malloc(sizeof(char**));
    *ptr = (char*) malloc(20*sizeof(char));
    int ind;
    int LTPU[2];
    int i=0;
    int val[10][2];
    int res[3];                 // RES => res[0] c'est le nombre rencontré
                                //     => res[1] c'est la valeur du caractere suivant
                                //     => res[2] c'est la position dans le fichier 
    ind = getWord(matrice, "LTPU");     // On cherche LTPU et on récupere sa position dans le fichier
    getNum(matrice, res, ind);          // On cherche ensuite le nombre juste après
    LTPU[0] = res[0];
    getNum(matrice, res, res[2]);       // Ainsi que celui juste apres car LTPU[X,Y]
    LTPU[1] = res[0];

    printf("LTPU : %d - %d\n\n",LTPU[0],LTPU[1]);
    do
    {
        i=0;
        getAWord(matrice, ptr, res);  // On cherche ensuite un mot (normalement PD ou PU)
        printf("\nDo a : %s\n",*ptr);
        do{
            getNum(matrice, res, res[2]);
            val[i/2][i%2] = res[0];
            i++;
        }while(res[1] ==44);    // tant qu'on ne rencontre pas de ;

        printTab(val, i/2);
    } while(checkEnd(matrice) ==0);

    printf("\nFin du fichier\n");                                   
                                                         
    return 0;
}

void printTab(int tab[][2], int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        printf("[%d;%d]\n",tab[i][0],tab[i][1]);
    }
}

int analyze_matrix(FILE* matrice) {
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
                    printf("_");
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
    return 1;
}

int getWord(FILE* matrice, char* word)
{
   int i=0;
   int end=0; 
   char c;
   char buff_word[50];

   if(matrice == NULL) {
       printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
       printf("Fin du programme\n");
       return -1;
   }
   else {
       do
       {
           c = fgetc(matrice);
           if(c == EOF) {       // Cas de la fin du fichier
               end = 1;
               printf("\nFin du fichier\n");
           }
           else if(c == 59){    // On veut pas du point virgule dans les mots
           }
           else if ((c >= 58 && c <= 126) || (c >=32 && c<=47 )) {  // si c'est un caractere
               buff_word[i] = c;        // On stocke le caractere
               buff_word[i+1] = '\0';   // On met le caractere fin de texte juste apres
               i++;                     // On avance dans le mot
           }
           else {                       // Si on arrive ici, ce n'est plus un caractere donc le mot est termine
               if(i != 0) {             // Si i different de 0, on a bien recuperer un mot, on le compare
   //                printf("we got a word : %s\n", buff_word);
                   if(strcmp(buff_word, word) == 0) {           // On compare le mot en cours avec le mot recherche
    //                   printf("We got the good word, at %d\n", ftell(matrice));
                       
                       return ftell(matrice)-1;                   // On retourne la position dans le fichier (-1 car on a su que c'était un mot au moment ou on a rencontrer autre chose qu'un caractere)
                    }
                   i=0;
               }
           }
       } while(!end);
   }
   return 0;                    // Si on a pas trouve le mot, on renvoie 0
}

void getAWord(FILE* matrice, char** ptr, int res[3])     // Variante de getWord qui va renvoyer le premier mot qu'il rencontre
{
   int i=0;
   int end=0; 
   char c;
   char buff_word[50];

   if(matrice == NULL) {
       printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
       printf("Fin du programme\n");
       exit(-1);
   }
   else {
       do
       {
           c = fgetc(matrice);
           if(c == EOF) {       // Cas de la fin du fichier
               end = 1;
               printf("\nFin du fichier\n");
           }
           else if(c == 59){    // On veut pas du point virgule dans les mots
           }
           else if ((c >= 58 && c <= 126) || (c >=32 && c<=47 )) {  // si c'est un caractere
               buff_word[i] = c;        // On stocke le caractere
               buff_word[i+1] = '\0';   // On met le caractere fin de texte juste apres
               i++;                     // On avance dans le mot
           }
           else {                       // Si on arrive ici, ce n'est plus un caractere donc le mot est termine
               if(i != 0) {             // Si i different de 0, on a bien recuperer un mot, on le compare
   //                printf("we got a word : %s\n", buff_word);
                       res[2] = ftell(matrice)-1;                   // On retourne la position dans le fichier (-1 car on a su que c'était un mot au moment ou on a rencontrer autre chose qu'un caractere)
                       strcpy(*ptr, buff_word);                            // On retourne le mot rencontre
                       return;
               }
           }
       } while(!end);
   }
   return;                    // Si on a pas trouve le mot, on renvoie 0
}


void getNum(FILE* matrice, int res[3], int pos)
{
    int j=0;
    int end=0; 
    char c;
    char buff_num[50];

    if(matrice == NULL) {
        printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
        printf("Fin du programme\n");
        exit(-1);
    }
    else {
        fseek(matrice, pos, SEEK_SET);  // On se repositionne a la position indiquee avant de commencer
        do
        {
            c = fgetc(matrice);
            if(c == EOF) {       // Cas de la fin du fichier
                end = 1;
                printf("\nFin du fichier\n");
            }
            else if(c >= 48 && c <=57) { // Si c'est un chiffre
                buff_num[j] = c;         // On stocke le chiffre
                buff_num[j+1] = '\0';    // On met le caractere fin de texte juste apres
                j++;                     // On passe au caractere d'apres
            }
            else {                       // Si on arrive ici, ce n'est plus un caractere donc le mot est termine
                if(j != 0) {             // Si i different de 0, on a bien recuperer un mot, on le compare
     //               printf("we got a number : %s\n", buff_num);
                    res[0] = atoi(buff_num);                   // On retourne la position dans le fichier
                    res[1] = c;                                 // ainsi que le dernier caractere rencontre afin de savoir si c'est une virgule ou un point virgulle
                    res[2] = ftell(matrice);                                 // la position dans le fichier
                    return;
                }
                j=0;
            }
        }while(!end);
    }
    printf("pwet\n");
}






int new_matrix_analyser(FILE* matrice) {
    char buff_word[100];
    char buff_num[20];
    int i=0,j=0;
    int end=0;
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
                printf("_");
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


