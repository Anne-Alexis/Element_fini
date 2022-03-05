#ifndef GEOM_HPP
#define GEOM_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include "echelle_couleur.hpp"
#include "parametres.hpp"

//
class triangle;
class polygone;

//CLASSE POINT

class point
{
    real m_x;
    real m_y;

    public:
        point(real,real);
        
        real& operator[](unsigned int);
        real operator[](unsigned int) const;
        point& operator-=(const point & b);
        point& operator+=(const point & b);
        point& operator*=(real x);
        point& operator/=(real x);

        point& transformation_affine(const triangle & t);
        point conjugue() const;
        real norme2() const;
};

point operator-(const point & a,const point & b);
point operator+(const point & a, const point & b);
point operator*(real, const point & b);
point operator*(const point & b, real);
point operator/(real, const point & b);
point operator/(const point & b, real);
real operator^(const point & a, const point & b); //renvoi le produit vectoriel projeté sur la normale au plan
point operator/(const point &a, const point &b); //division terme à terme
bool operator==(const point &a,const point &b);
bool operator!=(const point &a,const point &b);

//CLASSE POLYGONE

class polygone
{
    protected:
        std::vector<point> m_sommets;

    public:
        polygone();
        polygone(const std::vector<point> & s);

        std::vector<point> get_sommets() const;
        
        point& operator[](int);
        point operator[](int) const;
        point& operator()(int);
        point operator()(int) const;

        polygone& operator*=(real x);

        void draw(sf::RenderWindow&) const;

        int size() const;

        void remove(int i);
        int trouve_oreille() const;  
        void ajoute_point(point);
};

bool operator==(const polygone &, const polygone& );
bool operator!=(const polygone &, const polygone& );

//CLASSE TRIANGLE

class triangle : public polygone
{
    public:
        triangle(const std::vector<point> &); 

        triangle& transformation_affine(triangle & ref);
        
        real demi_perimetre() const;
        real aire() const;
        real rayon_cercle_inscrit() const;
        real rayon_cercle_circonscrit() const;
        point centre_cercle_circonscrit() const;
        point centre_cercle_inscrit() const;

        real qualite() const;

        void drawmediane(sf::RenderWindow& w) const;
        void draw_fill(real, real , real, std::pair<real,real> ,sf::RenderWindow &w) const;
};

//QUELQUES FONCTIONS

real longueur_points(const point & a, const point & b);
real produit_scalaire(const point & a, const point & b);

#endif