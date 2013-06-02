typedef struct Coord Coord;
struct Coord{
      int x;
      int y;
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
