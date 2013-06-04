#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pwet.h"

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
void resizeFigure(Shape** Figure, int size)
{
    //Shape** new = realloc(Figure,  size*sizeof(Shape*));
    realloc(Figure,  size*sizeof(Shape*));
    //return new;
}

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

void addVectorList(Coord *From, Coord *To, char* type, Vector** List, int size)
{
    List[size] = newVectorBis(From, To, type);
}

void copyCoord(Coord* From, Coord* To)
{
    To->x = From->x;
    To->y = From->y;
}

// Affichage d'un Vector
void printVector(Vector *v)
{
    if(v->type)
        printf("\nPU");
    else
        printf("PD");
    printf(" A[%d,%d] B[%d,%d]\n", v->a.x, v->a.y, v->b.x, v->b.y);
}

// Affichage d'un Coord
void printCoord(Coord *a)
{
    printf("Coord [%d,%d]\n", a->x, a->y);
}

void printShape(Shape *a)
{
    while(a->next != NULL)
    {
        printf("Shape [%d,%d]\n",a->point.x,a->point.y);
        a = a->next;
    }
}

void printFigure(Shape **a, int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        printf("Figure num %d\n",i);
        printShape(a[i]);
    }
}

int main(int argc, char* argv[])
{
    FILE* matrice = fopen(argv[1],"r");
    char **ptr=malloc(sizeof(char**));
    *ptr = (char*) malloc(20*sizeof(char));
 
    Vector **List=malloc(sizeof(Vector**));         // Tableau de pointeur de Vector
    *List = (Vector*) malloc(20*sizeof(Vector*));   // De taille : 20

    Shape **Figure = malloc(sizeof(Shape**));       // Tableau dynamique de pointeur de Figures
    
    int ind;
    Coord OldCoord;
    Coord TmpCoord;
    Shape* TmpShape;
    int i=0;
    int j=0;
    int good;
    int res[3];                 // RES => res[0] c'est le nombre rencontré
                                //     => res[1] c'est la valeur du caractere suivant
                                //     => res[2] c'est la position dans le fichier 

    ind = getWord(matrice, "LTPU");     // On cherche LTPU et on récupere sa position dans le fichier
    getNum(matrice, res, ind);          // On cherche ensuite le nombre juste après
    OldCoord.x = res[0];
    getNum(matrice, res, res[2]);       // Ainsi que celui juste apres car LTPU[X,Y]
    OldCoord.y = res[0];

    printf("LTPU [%d-%d]\n\n",OldCoord.x,OldCoord.y);
    do                                  // On passe d'action en action
    {
        i=0;
        getAWord(matrice, ptr, res);    // On cherche ensuite un mot (normalement PD ou PU)
        printf("We got : %s\n",*ptr);
        if(strcmp(*ptr,"PD")==0)
        {
            printf("j = %d\n",j);
            j++;
            resizeFigure(*Figure, j);
            Figure[j-1] = newShape();
            good = 1;
            TmpShape = Figure[j-1];
        }
        else
        {
            good = 0;
        }
        do{                             // On passe de points en points
            getNum(matrice, res, res[2]);
            TmpCoord.x = res[0];
            //val[i][0] = res[0];
            getNum(matrice, res, res[2]);
            TmpCoord.y = res[0];
            //val[i][1] = res[0];
            i++;
            addVectorList(&OldCoord,&TmpCoord,*ptr,List, i);

            if(good)
            {
                Figure[j-1]->point.x = TmpCoord.x;
                Figure[j-1]->point.y = TmpCoord.y;
                Figure[j-1]->next = newShape();
                Figure[j-1] = Figure[j-1]->next;
            }

            copyCoord(&TmpCoord, &OldCoord);
            printVector(List[i]);
        }while(res[1] ==44);            // tant qu'on ne rencontre pas de ;
        if(good)
            Figure[j-1] = TmpShape;
    } while(checkEnd(matrice) ==0);     // Tant qu'on ne rencontre pas le caractere "echap"

    printf("\nFin du fichier\n");                                   
 
    printFigure(Figure,j);

    return 0;
}

// Affiche un tableau de coordonnées à partir de sa taille
void printTab(int tab[][2], int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        printf("[%d;%d]\n",tab[i][0],tab[i][1]);
    }
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

