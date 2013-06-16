#ifndef H_PWET
#define H_PWET

int getWord(FILE* matrice, char* word, FILE* info, int onlyPrint);
void getAWord(FILE* matrice, char* ptr, int res[3]);
void getNum(FILE* matrice, int res[3], int pos);
int checkEnd(FILE* matrice);
Vector* newVector();
Shape* newShape();
void resizeFigure(Shape*** Figure, int size);
Vector* newVectorBis(Coord *From, Coord *To, char* type);
void addVectorList(Coord *From, Coord *To, char* type, Vector** List, int size);
void getRestOfFile(FILE* matrice, FILE* info);
void printTxtInFile(char* txt, FILE* file);
void recreateFile(Shape** Figure, int size, FILE* newFile);


#endif