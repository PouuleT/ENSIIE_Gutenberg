#include "../inc/struct.h"
#include "../inc/optimisation.h"
#include "../inc/outils.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

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