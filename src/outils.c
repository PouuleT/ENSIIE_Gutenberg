#include "../inc/struct.h"
#include "../inc/outils.h"
#include <math.h>

float dist(Coord a, Coord b)
{
    return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

// Calcule la distance entre deux points a et b
float distancePoints (Coord a, Coord b)
{
    return (sqrt(pow(b.x - a.x, 2)+pow(b.y - a.y, 2)));
}

// Retourne qui entre les points a ou b est le plus proche du point p
Coord projetePointSommets (Coord p, Coord a, Coord b)
{
    Coord res;
    if (distancePoints(p,a) <= distancePoints(p,b)){
        res = a;
    } else {
        res = b;
    }

    return res;
}

// Retourne le projeté orthogonal du point p sur le segment entre a et b
Coord projetePointSegment (Coord p, Coord a, Coord b)
{
    Coord res;
    // n et m sont les constantes dans l'équation de la droite qui passe par a et b : y = nx + m
    float n = (a.y - b.y)/(a.x - b.x);
    float m = b.y - n*(b.x);

    res.x = round((n*(p.y + (n*p.x)) +m)/(pow(n,2)+1));
    res.y = round((n*res.x)+m);

    // Si le projeté du point p ne se trouve pas sur le segment
    if (!(distancePoints(res, a) <= distancePoints(a, b)) || !(distancePoints(res, b) <= distancePoints(a, b)))
    {
        res = projetePointSommets(p, a, b); // On retourne le bord du segment le plus proche de p
    }

    return res;
}

// Will copy one Coord into another one
void copyCoord(Coord* From, Coord* To)
{
    To->x = From->x;
    To->y = From->y;
}