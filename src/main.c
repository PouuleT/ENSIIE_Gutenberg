#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include "../inc/struct.h"
#include "../inc/affichage.h"
#include "../inc/optimisation.h"
#include "../inc/outils.h"
#include "../inc/pwet.h"

int main(int argc, char* argv[])
{
    FILE* matrice = fopen(argv[1],"r");
    FILE* newFile;
    int onlyPrint;          // Tells if we just want to print info, or to optimize the file
    char *fileNew;
    Vector **List=malloc(100*sizeof(Vector*));         // Tableau de pointeur de Vector
    char *ptr=malloc(sizeof(char)*20);              // ptr va stocker les mots qu'on trouve dans le fichier
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

    printf("\nDistance totale PD : %.2f, PU : %.2f\n",totalPD, totalPU);
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

    fclose(matrice);

    return 0;
}
