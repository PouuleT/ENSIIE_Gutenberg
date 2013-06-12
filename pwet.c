#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pwet.h"

float totalPD=0.0;
float totalPU=0.0;

float dist(Coord a, Coord b)
{
    return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

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

// Will copy one Coord into another one
void copyCoord(Coord* From, Coord* To)
{
    To->x = From->x;
    To->y = From->y;
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

// Affichage d'un Vector
void printVector(Vector *v)
{
    float distParcourue = dist(v->a,v->b);
    if(v->type)
    {
        printf("\nPU");
        totalPU += distParcourue;
    }
    else
    {
        printf("PD");
        totalPD += distParcourue;
    }
    printf(" A[%d,%d] B[%d,%d] DIST : %f\n", v->a.x, v->a.y, v->b.x, v->b.y, distParcourue );
}

// Affichage d'un Coord
void printCoord(Coord *a)
{
    printf("Coord [%d,%d]\n", a->x, a->y);
}

// Will print the Coord that is inside a Shape
void printShapeUnit(Shape *a)
{
    printf("Shape [%d,%d]\n",a->point.x,a->point.y);
}

// Affiche une Shape
void printShape(Shape *a)
{
    Shape* Tmp = a;        
    while(Tmp->next != NULL)      // On parcourt toute la Shape
    {
        printShapeUnit(Tmp);      // On affiche un élément
        Tmp = Tmp->next;            // On passe à l'élement d'après
    }
}

// Affiche une Figure (avec sa taille)
void printFigure(Shape **Figure, int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        printf("Figure num %d\n",i);
        printShape(Figure[i]);
    }
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

// Ecrit les coordonnées d'une Shape séparés d'une virgule dans le fichier
void printShapeUnitInFile(Shape* unit, FILE* file)
{
    fprintf(file, "%d,%d", unit->point.x,unit->point.y);
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

// Calcule la distance entre deux points a et b
float distancePoints (Coord a, Coord b)
{
    return (sqrt(pow(b.x - a.x, 2)+pow(b.y - a.y, 2)));
}

// Retourne qui entre les points a ou b est le plus proche du point p
Coord projetePointSommets (Coord p, Coord a, Coord b)
{
    Coord res;
    if (distancePoints(p,a) <= distancePoints(p,b)){
        res = a;
    } else {
        res = b;
    }

    return res;
}

// Retourne le projeté orthogonal du point p sur le segment entre a et b
Coord projetePointSegment (Coord p, Coord a, Coord b)
{
    Coord res;
    // n et m sont les constantes dans l'équation de la droite qui passe par a et b : y = nx + m
    float n = (a.y - b.y)/(a.x - b.x);
    float m = b.y - n*(b.x);

    res.x = round((n*(p.y + (n*p.x)) +m)/(pow(n,2)+1));
    res.y = round((n*res.x)+m);

    // Si le projeté du point p ne se trouve pas sur le segment
    if (!(distancePoints(res, a) <= distancePoints(a, b)) || !(distancePoints(res, b) <= distancePoints(a, b)))
    {
        res = projetePointSommets(p, a, b); // On retourne le bord du segment le plus proche de p
    }

    return res;
}

int main(int argc, char* argv[])
{
    FILE* matrice = fopen(argv[1],"r");
    char *fileNew = malloc(sizeof(char)*(strlen(argv[1])+4));
    strcpy(fileNew,"new_"); 
    strcat(fileNew,argv[1]);                        // Le nouveau fichier sera nommé à l'identique, avec new_ devant
    FILE* newFile = fopen(fileNew,"w");             // On ouvre le nouveau fichier créé
    char *ptr=malloc(sizeof(char)*20);              // ptr va stocker les mots qu'on trouve dans le fichier
 
    Vector **List=malloc(100*sizeof(Vector*));         // Tableau de pointeur de Vector
//    List = (Vector*) malloc(100*sizeof(Vector*));  // De taille : 20

    Shape **Figure = malloc(sizeof(Shape*));       // Tableau dynamique de pointeur de Figures

    int ind;
    Coord OldCoord;
    Coord TmpCoord;
    Shape* TmpShape;
    int i=0;
    int j=0;
    int first = 1;
    int good = 2;
    int res[3];                 // RES => res[0] c'est le nombre rencontré
                                //     => res[1] c'est la valeur du caractere suivant
                                //     => res[2] c'est la position dans le fichier 

    ind = getWord(matrice, "LTPU", newFile);     // On cherche LTPU et on récupere sa position dans le fichier
    getNum(matrice, res, ind);          // On cherche ensuite le nombre juste après
    OldCoord.x = res[0];                // On le stock
    getNum(matrice, res, res[2]);       // Ainsi que celui juste apres car LTPU[X,Y]
    OldCoord.y = res[0];                // On le stock

    printf("LTPU [%d-%d]\n",OldCoord.x,OldCoord.y);
    do                                          // On passe d'action en action (de PU en PD etc ...)
    {
        i=0;
        getAWord(matrice, ptr, res);            // On cherche ensuite un mot (normalement PD ou PU) sinon il y a un problème
        printf("\nWe got : %s\n",ptr);
        if(strcmp(ptr,"PU")==0 || first == 1)  // Si c'est un PU (ou si first ==1 et dans ce cas là le mot était LTPU), c'est le début d'une Shape
        {
            printf("Shape num %d\n",j);
            j++;                                // On augmente la taille de la Figure
            resizeFigure(&Figure, j+1);           // On rajoute une case à la Figure
            Figure[j-1] = newShape();           // On met une Shape dans la nouvelle case créée
            TmpShape = Figure[j-1];             // On stock l'adresse de la première Shape pour la remettre dans l'ordre à la fin
            if(first)
            {
                good = 2;                       // On indique qu'on est prêt à rajouter les autres Shape et qu'à la fin on remette tout en ordre
                Figure[j-1]->point.x = OldCoord.x;  // On stock le nombre LTPU (x)
                Figure[j-1]->point.y = OldCoord.y;  // On stock le nombre LTPU (y)
                Figure[j-1]->next = newShape();     // On prépare la prochaine Shape
                Figure[j-1] = Figure[j-1]->next;    // On décale le debut de la figure
            }
            else
                good = 1;                       // On indique qu'on est prêt à rajouter les autres Shape
            first = 0;                          // On indique que ce n'est plus le départ
        }
        else
        {
            good = 2;                           // Si ce n'est pas un PU ni le départ, alors c'est un PD
        }
        do{                                     // On passe de points en points
            getNum(matrice, res, res[2]);       // On prend le nombre
            TmpCoord.x = res[0];                // On le stock
            getNum(matrice, res, res[2]);       // On prend le nombre
            TmpCoord.y = res[0];                // On le stock
            i++;                                // On augmente la taille de la liste de Vector
            addVectorList(&OldCoord,&TmpCoord,ptr,List, i);    // Et on rajoute le nouveau Vector à la liste

            if(good!=0)
            {
                Figure[j-1]->point.x = TmpCoord.x;  // On stock le nombre (x)
                Figure[j-1]->point.y = TmpCoord.y;  // On stock le nombre (y)
                Figure[j-1]->next = newShape();     // On prépare la prochaine Shape
                Figure[j-1] = Figure[j-1]->next;    // On décale le debut de la figure
            }

            copyCoord(&TmpCoord, &OldCoord);        // On copie l'ancien point pour pouvoir le stocker dans un Vector (on crée un Vector à chaque nouveau point Vector [ ancien point ; nouveau point ]
            printVector(List[i]);                   // On affiche le Vector qu'on vient de créer
        }while(res[1] ==44);            // On continue tant qu'on ne rencontre pas de ;

        if(good==2)
        {
            printf("\nWe got the whole shape, on remet dans l'ordre\n");
            Figure[j-1] = TmpShape;     // On remet la première Shape pour tout remettre dans l'ordre
        }

    } while(checkEnd(matrice) ==0);     // Tant qu'on ne rencontre pas le caractere "echap"

    printFigure(Figure,j);              // On affiche la Figure

    printf("\nTODO : Maintenant qu'on a recuperer tous les points, on optimise\n");

    printf("\nMaintenant que les points sont dans un ordre optimal, on reforme la structure et on réécrit tout dans le nouveau fichier\n");

    recreateFile(Figure, j, newFile);

    printf("\nEt maintenant on récupère le reste du fichier et on le met dans le nouveau fichier pour reformer un fichier prn correct\n");

    getRestOfFile(matrice, newFile);

    printf("\nDistance totale PD : %f, PU : %f\n",totalPD, totalPU);
    printf("\nNouveau fichier créé, on ferme tout et on quitte. THE END.\n\n");                                   
 
    fclose(newFile);
    fclose(matrice);
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

