#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../inc/struct.h"
#include "../inc/affichage.h"

// Affichage d'un Vector
void printVector(Vector *v)
{
    // float distParcourue = dist(v->a,v->b);
    // if(v->type)
    // {
    //     printf("\nPU");
    //     totalPU += distParcourue;
    // }
    // else
    // {
    //     printf("PD");
    //     totalPD += distParcourue;
    // }
    // printf(" A[%d,%d] B[%d,%d] DIST : %f\n", v->a.x, v->a.y, v->b.x, v->b.y, distParcourue );
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

// Ecrit les coordonnées d'une Shape séparés d'une virgule dans le fichier
void printShapeUnitInFile(Shape* unit, FILE* file)
{
    fprintf(file, "%d,%d", unit->point.x,unit->point.y);
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