#ifndef H_STRUCT
#define H_STRUCT

typedef struct Coord Coord;
struct Coord{
      int x;
      int y;
};

typedef struct Shape Shape;
struct Shape{
    Coord point;
    Shape* next;
};

typedef struct Vector Vector;
struct Vector{
    // type :
    // 0 = Vecteur de tracé (PD)
    // 1 = Vecteur de déplacement (PU)
    int type;
    Coord a;
    Coord b;
//    Vector* next;
};

#endif