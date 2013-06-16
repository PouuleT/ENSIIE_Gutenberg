#ifndef H_OPTIMISATION
#define H_OPTIMISATION

void optiPremierPoint(Shape **Figure, int size);
int findNearestPointInShape(Coord* Point, Coord* nearestPoint, Shape* Forme, int bestDistance);
void findNearestPointInFigure(Coord* Point, Shape** Figure, int size, Shape** newFigure, int newFigureID);
Shape** optimiseFigure(Shape **Figure, int size);

#endif