#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include "../inc/struct.h"
#include "../inc/affichage.h"
#include "../inc/outils.h"

// Will find the word in parameters
int getWord(FILE* matrice, char* word, FILE* info)
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
           fprintf( info, "%c", c);

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
                   if(strcmp(buff_word, word) == 0) {           // On compare le mot en cours avec le mot recherche

                        fseek(info, -(strlen(word)+1), SEEK_CUR);   // On se repositionne dans le nouveau fichier avant le mot qu'on cherchait vu qu'on veut pas forcément le mettre dans le nouveau fichier
                       return ftell(matrice)-1;                   // On retourne la position dans le fichier (-1 car on a su que c'était un mot au moment ou on a rencontrer autre chose qu'un caractere)
                    }
                   i=0;
               }
           }
       } while(!end);
   }
   return 0;                    // Si on a pas trouve le mot, on renvoie 0
}

// Prend n'importe quel mot après la position présente dans res[3]
void getAWord(FILE* matrice, char* ptr, int res[3])     // Variante de getWord qui va renvoyer le premier mot qu'il rencontre
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
                       strcpy(ptr, buff_word);                            // On retourne le mot rencontre
                       return;
               }
           }
       } while(!end);
   }
   return;                    // Si on a pas trouve le mot, on renvoie 0
}

// Prend le mot juste après la position présente dans res
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
                    res[0] = atoi(buff_num);                   // On retourne la position dans le fichier
                    res[1] = c;                                 // ainsi que le dernier caractere rencontre afin de savoir si c'est une virgule ou un point virgulle
                    res[2] = ftell(matrice);                                 // la position dans le fichier
                    return;
                }
                j=0;
            }
        }while(!end);
    }
    printf("Problem in getNum\n");
}

// Check si c'est la fin des operations
int checkEnd(FILE* matrice)
{
    char word;

    word = fgetc(matrice);          // On récupère le caractère suivant
    fseek(matrice, -1, SEEK_CUR);   // On se repositionne a la position indiquee avant de commencer au cas où c'est pas la fin
    if(word == 27)                  // Si c'est le caractère "echap", c'est qu'il n'y a plus de PU/PD, on arrête
        return 1;
    else                            // Sinon on continue
        return 0;
}

// Allocation et initialisation d'un nouveau Vector
Vector* newVector()
{
    Vector* new = malloc(sizeof(Vector));
    return new;
}

// Allocation et initialisation d'une nouvelle Shape
Shape* newShape()
{
    Shape* new = malloc(sizeof(Shape));
    return new;
}

// Allocation et initialisation d'une nouvelle ShapeStar
void resizeFigure(Shape*** Figure, int size)
{
    //Shape** new = realloc(Figure,  size*sizeof(Shape*));
    *Figure = realloc(*Figure,  size*sizeof(Shape*));
    if(*Figure==NULL)
    {
        printf("Problem when reallocationg the Figure ...");
        exit(-1);
    }
    printf("pwet\n");
}

// Va créer un nouveau Vector et le remplir
Vector* newVectorBis(Coord *From, Coord *To, char* type)
{
    Vector* new = newVector();
    if(strcmp(type,"PU")==0)
    {
        new->type = 1;
    }
    else if(strcmp(type,"PD")==0)
    {
        new->type = 0;
    }
    else
    {
        printf("\n!!!!!!! Type inconnu : %s, on arrete ici\n\n",type);
        exit(-1);
    }

    copyCoord(From,&(new->a));
    copyCoord(To,&(new->b));

    return new;
}

// Crée yb biyveay Vector et le rajoute le Vecotr dans la liste
void addVectorList(Coord *From, Coord *To, char* type, Vector** List, int size)
{
    List[size] = newVectorBis(From, To, type);
}

// Pour récupérer la fin du fichier et la mettre dans le nouveau fichier
void getRestOfFile(FILE* matrice, FILE* info)
{
    char c;
    int end=0;

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
            fprintf(info, "%c", c);
        }while(!end);
    }
}

// Ecrit une chaine de caractere dans le fichier
void printTxtInFile(char* txt, FILE* file)
{
    fprintf(file, "%s", txt);
}

// Pour recréer le contenu du fichier (LTPU, PU, PD) dans newFile
void recreateFile(Shape** Figure, int size, FILE* newFile)
{
    printf("...\n");
    int i;
    Shape* Tmp;

    for(i=0;i<size;i++)                 // On passe de Shapes en Shapes
    {
        Tmp = Figure[i];
        if(i==0)
        {
            printTxtInFile("LTPU", newFile);

        }
        else
            printTxtInFile("PU", newFile);

        printShapeUnitInFile(Tmp, newFile);
        printTxtInFile(";PD", newFile);

        Tmp = Tmp->next;

        while(Tmp->next != NULL)  // On passe de points en points à l'intérieur des Shapes
        {
            printShapeUnitInFile(Tmp, newFile);
            Tmp = Tmp->next;
            if(Tmp->next != NULL)
                printTxtInFile(",", newFile);
        }
        printTxtInFile(";", newFile);
    }

}








//int new_matrix_analyser(FILE* matrice) {
//    char buff_word[100];
//    char buff_num[20];
//    int i=0,j=0;
//    int end=0;
//    char c;
//
//    if(matrice == NULL) {
//        printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
//        printf("Fin du programme\n");
//        return -1;
//    }
//    else {
//        printf("Analyse du fichier de matrice ...\n ");
//        do
//        {
//            c = fgetc(matrice);
//            if(c == EOF) {
//                end = 1;
//                printf("\nFin du fichier\n");
//            }
//            else if(c >= 48 && c <=57) {
//                buff_num[j] = c;
//                buff_num[j+1] = '\0';
//                j++;
//                if(i != 0) {
//                    printf("1 we got a string : %s\n", buff_word);
//                    i=0;
//                }
//            }
//            else if ((c >= 58 && c <= 126) || (c >=32 && c<=47 )) {
//                buff_word[i] = c;
//                buff_word[i+1] = '\0';
//                i++;
//                if(j != 0) {
//                    printf("2 we got a number : %d\n", atoi(buff_num));
//                    j=0;
//                }
//            }
//            else if(c == 27) {
//                printf("_");
//                if(j != 0) {
//                    printf("3 we got a number : %d\n", atoi(buff_num));
//                    j=0;
//                }
//                else if(i != 0) {
//                    printf("4 we got a string : %s\n", buff_word);
//                    i=0;
//                }
//            }
//            else {
//                if(j != 0) {
//                    printf("5 we got a number : %d\n", atoi(buff_num));
//                    j=0;
//                }
//                else if(i != 0) {
//                    printf("6 we got a string : %s\n", buff_word);
//                    i=0;
//                }
//
//                printf("unknown : %c - %d\n",c, c);
//            }
////            getchar();
//        } while (!end);
//    }
//    return 1;
//}

//int analyze_matrix(FILE* matrice) {
//    int end=0;
//    int k=0;
//    char c;
//
//    if(matrice == NULL) {
//        printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
//        printf("Fin du programme\n");
//        return -1;
//    }
//    else {
//        printf("Analyse du fichier de matrice ...\n ");
//        do
//        {
//            c = fgetc(matrice);
//            if(c == EOF) {
//                end = 1;
//                printf("\nFin du fichier\n");
//            }
//            else {
//                if(c == 27)
//                    printf("_");
//                else if(c == 1)
//                    printf("[]");
//                else
//                {
//                    printf("%c", c);
//                }
//                k++;
//            }
//        } while (!end);
//        printf("\nLe fichier comporte %d lignes\n", k);
//    }
//    printf("Done\n");
//    return 1;
//}

