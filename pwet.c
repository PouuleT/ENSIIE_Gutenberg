#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include "pwet.h"

float totalPD=0.0;
float totalPU=0.0;

float dist(Coord a, Coord b)
{
    return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

// Will find the word in parameters
int getWord(FILE* matrice, char* word, FILE* info, int onlyPrint)
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
           if(!onlyPrint)
           {
                fprintf( info, "%c", c);
           }

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
                        if(!onlyPrint)
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
    printShapeUnit(Tmp);
    /*while(Tmp->next != NULL)      // On parcourt toute la Shape
    {
        printShapeUnit(Tmp);      // On affiche un élément
        Tmp = Tmp->next;            // On passe à l'élement d'après
    }*/
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

// Optimisation à partir du 1er point de chaque figure
void optiPremierPoint(Shape **Figure, int size)
{

  // Point de départ du laser
  Coord pointDepart;
  // On débute l'algo en prenant comme point de départ, le 1er point de la figure 0
  copyCoord(&Figure[0]->point, &pointDepart);
  int numFigureDepart = 0;

  int numFigureProche;
  int distFigureProche = INT_MAX;

  // Variables temporaires pour le tri
  Shape* tmp;
  int distTmp;

  int i;
  int permutation = 1;
  printFigure(Figure,size);
  while (permutation == 1)
  {
    permutation = 0 ;

    for (i = numFigureDepart + 1; i < size ; i++)
    {
      // Pour chaque figure, on calcule la distance avec le point de départ du laser
      distTmp = dist(pointDepart, Figure[i]->point);
      if (distTmp < distFigureProche)
      {

        // On conserve la distance et le numéro de la figure
        distFigureProche = distTmp;
        numFigureProche = i;
        permutation = 1 ;
      }
    }

    if (permutation == 1)
    {

      // La figure la plus proche de la figure de départ est enregistrer en n+1 de la figure de départ
      tmp = Figure[numFigureDepart + 1];
      Figure[numFigureDepart + 1] = Figure[numFigureProche];
      Figure[numFigureProche] = tmp;

      printf("Point de départ [%d,%d] - Point le plus proche [%d,%d] : %d \n", Figure[numFigureDepart]->point.x, Figure[numFigureDepart]->point.y, Figure[numFigureDepart + 1]->point.x, Figure[numFigureDepart + 1]->point.y, distFigureProche);

      // Le nouveau point de départ est le 1er point de la figure que l'on vient de trouver
      copyCoord(&Figure[numFigureDepart + 1]->point, &pointDepart);
      numFigureDepart++;
      distFigureProche = INT_MAX;
    }
  }
  printFigure(Figure,size);
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

// Fonction retourne 1 si a == b, 0 sinon
int isCoordEqual(Coord a, Coord b)
{
    if((a.x == b.x) && (a.y == b.y))
        return 1;
    else
        return 0;
}

// Fonction qui doit mettre Point en tant que point de depart de la Shape* Forme
void changeBeginingShape(Coord Point, Shape **Forme)
{
    Shape* Tmp = *Forme;
    Shape* endChain = NULL;
    Shape* newStart = NULL;
    Shape* newEnd;
    Shape* before;
    int end=0;

    printf("\nEnter changeBeginingShape, Shape : \n");
    printShape(*Forme);
    printf("To begin with the point : \n");
    printCoord(&Point);
    // Si Point est le premier element de la Forme, c'est que la forme est dans le bon ordre et tout est ok
    if(isCoordEqual(Point, Tmp->point))
    {
        printf("Tout est déjà dans l'ordre, rien besoin de changer\n");
        return;
    }
    else
    {
        before = Tmp;
        Tmp = Tmp->next;
        // A la recherche de l'élément recherché
        while((Tmp->next != NULL)&&(end == 0))
        {
            if(isCoordEqual(Point, Tmp->point)) // Si on a trouvé l'élément cherché
            {    
                newEnd = before;
                newStart = Tmp;
                end =1;
            }
            before = Tmp;
            Tmp = Tmp->next;
        }
        end = 0;
        // A la recherche du premier élément qui est en double à la fin
        while((Tmp->next != NULL) && (end == 0))
        {
            if(isCoordEqual((*Forme)->point, Tmp->point)) // Si on a trouvé l'élément de la fin
            {
                endChain = before;
                end = 1;
            }
            else
            {
                before = Tmp;
                Tmp = Tmp->next;
            }
        }
        // On a tous les éléments, on reforme la chaine
        newEnd->next = Tmp;
        endChain->next = *Forme;
    }
    copyCoord(&Point,&(Tmp->point));
    Tmp = Tmp->next;
    copyCoord(&Point,&(Tmp->point));
    printf("\nEnd of changeBeginingShape with the new one : \n");
    printShape(newStart);

    *Forme = newStart;
}


int findNearestPointInShape(Coord* Point, Coord* nearestPoint, Shape* Forme, int bestDistance)
{
    Shape* Tmp = Forme;             // On copie locallement l'adresse de la Shape pour pouvoir naviguer dedans sans rien changer
//    Coord nearestPoint;
    int min = bestDistance, distance;

    printf("Shape : Begin\n");
    printf("Shape : We're looking for the nearest point of : \n");
    printCoord(Point);

    while(Tmp->next != NULL)        // On parcourt toute la Shape
    {
        distance = dist(Tmp->point,*Point); // On calcule la distance entre le point courant et le point cherché
        printf("Nouvelle distance : %d\n",distance);
        if(( distance < min ) || (min == -1))                 // Si c'est moins ou si c'est la première fois, on update,
        {
            printf("Shape : C'est moins %d < %d , on update\n",distance,min);
            min = distance;
            copyCoord(&(Tmp->point),nearestPoint);    // On met en stock le point le plus proche
            printf("Shape : Nouveau point le plus proche : \n");
            printCoord(nearestPoint);
        }
        else                                // Sinon on continue
        {
        }
        printf("Shape : on passe a l'element d'apres\n");
        Tmp = Tmp->next;            // On passe à l'élement d'après
    }
    if(min == -1)
    {
        printf("Shape : Big problem findNearestPointInShape");
        exit(-1);
    }
    printf("Shape : End\n");
    return min;
}

void findNearestPointInFigure(Coord* Point, Shape** Figure, int size, Shape** newFigure, int newFigureID)
{
    int i, id;
    int min = -1, distance;
    Coord nearestPoint;

    printf("Figure : Begin\n");
    printf("Figure : We're looking for the nearest point of : \n");
    printCoord(Point);
    
    for(i=0;i<size;i++)
    {
        printf("Figure : step %d with Figure of size %d\n",i,size);
        // La fonction va récupérer le point le plus proche de Point dans la Shape Figure[i], retourner la distance minimale trouvée et mettre à jour nearestPoint avec le point en question
        distance = findNearestPointInShape(Point, &nearestPoint, Figure[i], min);
        if((distance < min) || (min == -1))                                             // Si la nouvelle distance est plus petite ou si c'est la première fois
        {
            printf("Figure : C'est moins %d < %d, on update\n",distance, min);
            min = distance;                                                             // On update le min
            printf("Figure : Nouveau point le plus proche de la shape num %d : \n",i);
            printCoord(&nearestPoint);
            id = i;
        }
        printf("Figure : on passe a l'element d'apres\n");
    }

    printf("Figure : Tous les elements ont été traités, on crée la nouvelle Figure\n");
    printf("D'abord on la met dans le bon ordre\n");
    changeBeginingShape(nearestPoint, &(Figure[id]));

    printf("On la transfert de Figure à newFigure\n");
    newFigure[newFigureID]=Figure[id];
    // Si la Shape trouvée est la dernière de la liste, on ne fait rien, le dernier élément est supprimé
    if(id == size)
    {
    }
    // Sinon, on met le dernier élément de la Figure à la place de la Shape trouvée
    else
    {
        Figure[id] = Figure[size-1];
    }


    if(min == -1)
    {
        printf("Figure : Big problem findNearestPointInShape");
        exit(-1);
    }
    printf("On met à jour le Point\n");
    copyCoord(&nearestPoint,Point);
    printf("Figure : End\n");
}

Shape** optimiseFigure(Shape **Figure, int size)
{
    Shape **newFigure = malloc(size*sizeof(Shape *));

    int i, originalSize = size;
    Coord* Point = malloc(sizeof(Coord));
    Point->x = 0;
    Point->y = 0;

    printf("\nWe enter in optimiseFigure\n");
    for(i=0;i<originalSize;i++)
    {
        printf("optimiseFigure step %d with Figure of size %d\n",i,size);
        // Cette fonction va chercher le point le plus proche de Point dans l'ensemble des points de Figure
        // Elle va ensuite rajouter ce point et sa Shape associé dans newFigure
        // Et supprimser le point et sa Shape associée de Figure (pour le pas traiter cette Shape à la prochaine étape
        // Et copier le point le plus proche dans Point
        findNearestPointInFigure(Point, Figure, size, newFigure, i);
        printf("\nBack in optimiseFigure\n");
        //
        size--;                             // Une figure vient d'être traitée, alors on diminue la taille et on recommence
    }

    free(Point);
    
    printf("\nEnd of optimiseFigure\n");

    return newFigure;
}

















/* 
 *
 *                      MAIN FUNCTION
 *
 */

int main(int argc, char* argv[])
{
    FILE* matrice = fopen(argv[1],"r");

    char *fileNew = malloc(sizeof(char)*(strlen(argv[1])+4));
    strcpy(fileNew,"new_");
    strcat(fileNew,argv[1]);                        // Le nouveau fichier sera nommé à l'identique, avec new_ devant
             // ptr va stocker les mots qu'on trouve dans le fichier
    
	FILE* newFile;
    int onlyPrint;          // Tells if we just want to print info, or to optimize the file
    char *fileNew;
    Vector **List=malloc(100*sizeof(Vector*));         // Tableau de pointeur de Vector

    Shape **Figure = malloc(sizeof(Shape*));       // Tableau dynamique de pointeur de Figures
    Shape **newFigure;

    int ind;
    Coord OldCoord;
    Coord TmpCoord;
    Shape* TmpShape;
    int i=0;
    int j=0;
    int k;
    int first = 1;
    int good = 2;
    int res[3];                 // RES => res[0] c'est le nombre rencontré
                                //     => res[1] c'est la valeur du caractere suivant
                                //     => res[2] c'est la position dans le fichier

    if((argc==3) && (strcmp(argv[2],"print")==0))
    {
        onlyPrint = 1;
    }
    else
    {
        onlyPrint = 0;
        fileNew = malloc(sizeof(char)*(strlen(argv[1])+4));
        strcpy(fileNew,"new_"); 
        strcat(fileNew,argv[1]);                        // Le nouveau fichier sera nommé à l'identique, avec new_ devant

        newFile = fopen(fileNew,"w");             // On ouvre le nouveau fichier créé
    }


    ind = getWord(matrice, "LTPU", newFile, onlyPrint);     // On cherche LTPU et on récupere sa position dans le fichier
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

            copyCoord(&TmpCoord, &OldCoord);        // On fais une copie l'ancien point pour pouvoir le stocker dans un Vector (on crée un Vector à chaque nouveau point Vector [ ancien point ; nouveau point ]
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

    optiPremierPoint(Figure,j);
    printf("\nMaintenant que les points sont dans un ordre optimal, on reforme la structure et on réécrit tout dans le nouveau fichier\n");

    recreateFile(Figure, j, newFile);

    if(onlyPrint == 1)
    {
        newFigure = Figure;
    }
    else
    {
        printf("\nMaintenant qu'on a recuperer tous les points, on optimise\n");
    
        newFigure = optimiseFigure(Figure, j);
        
        printf("\nMaintenant qu'on a optimisé, on affiche le nouveau resultat!\n");
    
        printFigure(newFigure,j);              // On affiche la Figure
    
        printf("\nMaintenant que les points sont dans un ordre optimal, on reforme la structure et on réécrit tout dans le nouveau fichier\n");
    
        recreateFile(newFigure, j, newFile);
    
        printf("\nEt maintenant on récupère le reste du fichier et on le met dans le nouveau fichier pour reformer un fichier prn correct\n");
    
        getRestOfFile(matrice, newFile);
        fclose(newFile);
    } 
    
    printf("\nDistance totale PD : %f, PU : %f\n",totalPD, totalPU);
    printf("\nNouveau fichier créé, on ferme tout et on quitte. THE END.\n\n"); 

    for(k=0;k<i;k++)
    {
        free(List[k]);
    }
    free(List);
    for(k=0;k<j;k++)
    {
        free(newFigure[k]);
    }
    free(newFigure);

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

