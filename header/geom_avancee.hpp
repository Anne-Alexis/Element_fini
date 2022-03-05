#ifndef GEOM_AVANCEE_HPP
#define GEOM_AVANCEE_HPP

#include "geom.hpp"
#include "maillage.hpp"

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

class arete;

bool estDedans(const point& x, const polygone& pol);
double integrale_angle(const point& x, const point& a, const point& b);
point normale_sortante(const triangle &, const arete &);

class droite
{
    point m_vecteur_directeur;
    point m_origine;

    public:
        droite(const point&, const point&);
        point get_origine();
        point get_vdirecteur();
        point get_point(double t); //renvoie le point de paramètre t

        void draw(sf::RenderWindow& , double taille);
};

point intersection_droites(droite&, droite&);

#endif