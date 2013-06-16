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