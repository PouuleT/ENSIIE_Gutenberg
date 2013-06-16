#ifndef H_OUTILS
#define H_OUTILS

float dist(Coord a, Coord b);
float distancePoints (Coord a, Coord b);
Coord projetePointSommets (Coord p, Coord a, Coord b);
Coord projetePointSegment (Coord p, Coord a, Coord b);
void copyCoord(Coord* From, Coord* To);

#endif
