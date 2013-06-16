#ifndef H_AFFICHAGE
#define H_AFFICHAGE

#include <stdlib.h>
#include <stdio.h>

void printVector(Vector *v);
void printCoord(Coord *a);
void printShapeUnit(Shape *a);
void printShape(Shape *a);
void printFigure(Shape **Figure, int size);
void printTxtInFile(char* txt, FILE* file);
void printShapeUnitInFile(Shape* unit, FILE* file);
void printTab(int tab[][2], int size);

#endif