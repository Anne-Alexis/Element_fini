#ifndef MAILLAGE_HPP
#define MAILLAGE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <utility>
#include "geom.hpp"
#include "geom_avancee.hpp"
#include "parametres.hpp"

class arete;
class maille_triangle;
class sommet;

//CLASSE MAILLAGE

class maillage
{
    std::vector<maille_triangle> m_triangles; //Liste des mailles
    std::vector<sommet> m_sommets; //Liste des sommets
    std::vector<arete> m_aretes; //Listes des aretes
    //Chaque élément contient la liste des autres éléments auxquel il est lié, classé dans le bon ordre

    int m_nb_aretes_interieures;
    real m_h; //Taille du maillage
    void raffinage_par2_tri(int);
    
    public:
        
        maillage(polygone poly, real h);
        //renvoie true si le triangle a été subdivisé, false si il était déjà assez petit

        sommet& get_sommet(int);
        sommet get_sommet(int) const;
        maille_triangle& get_triangle(int);
        maille_triangle get_triangle(int) const;
        arete& get_arete(int);
        arete get_arete(int) const;
        int nb_triangles() const;
        int nb_sommets() const;
        int nb_aretes() const;
        int nb_aretes_interieures() const;

        bool ajoute_triangle(const triangle& tr, int num_tri=-1, std::vector<int> num_pts={-1,-1,-1}, std::vector<int> num_aretes={-1,-1,-1} ) ;
        //bool ajoute_sommet(const sommet&);
        //bool ajoute_arete(const arete&);
        void vide_triangle(int); //Supprime les liens sans décaler le reste des triangles
        void enleve_triangle(int);
        void enleve_arete(int); //Supprime les aretes vides
        void ajuste_aretes_interieures(); //Ajuste le statut interieur des aretes

        void draw(sf::RenderWindow&) const;
        void ameliorer_delaunay();
        bool arete_commune(int i, int j, int &, int &) const;

        void raffinage_par2();
        bool arete_meme_triangle(int , int , int& ) const;
};

bool est_delaunay(const triangle &, const point &);

/////////MAILLE_TRIANGLE///////////

class maille_triangle: public triangle
{
    protected:
        std::vector<int> m_num_sommets; //Liste des sommets classés dans l'ordre
        std::vector<int> m_num_aretes; //Liste des aretes classées correctement
        int m_num; //Le numéro de la maille

    public:
        maille_triangle(const std::vector<point> &, int);
        maille_triangle(const triangle &, int);

        int nb_sommets() const; //Normalement renvoi 3
        int nb_aretes() const; //Normalement renvoi 3*
        int get_num() const;

        int& get_nsommet(int); //Renvoi le num du sommet
        int get_nsommet(int) const;
        int& get_narete(int); //Renvoi le num de l'arete
        int get_narete(int) const;

        void ajoute_arete(int);
        void ajoute_sommet(int);
        void change_num(int);

        void enleve_arete(int);
        void enleve_sommet(int);
        void vide();
};

//////////////SOMMET//////////////

class sommet: public point
{
    protected:
        std::vector<int> m_num_triangles; //Liste des sommets classés dans l'ordre
        std::vector<int> m_num_aretes; //Liste des sommets classés dans l'ordre
        int m_num; //Le numéro du sommet

    public:
        sommet(real, real, int);
        sommet(const point&, int);

        int nb_triangles() const; 
        int nb_aretes() const;
        int get_num() const;
        
        int& get_narete(int); //Renvoi le num de l'arete
        int get_narete(int) const;
        int& get_ntriangle(int); //Renvoi le num du triangle
        int get_ntriangle(int) const;

        void ajoute_arete(int);
        void ajoute_triangle(int);
        void change_num(int);

        void enleve_arete(int);
        void enleve_triangle(int);
};


/////////ARETE///////////

class arete
{
    //Les deux points de l'arete
    point m_a; 
    point m_b;

    int m_num; //Le numéro de l'arete

    bool m_inter; //True si c'est une face intérieure, false sinon

    int m_num_a; //Les numéros des sommets a et b dans le maillage
    int m_num_b;

    std::vector<int> m_triangles; //vecteur des numéros de triangles auxquels l'arête appartient;
    real m_taille;

    public:
    
    arete(const point &a, const point &b, int na=-1, int nb=-1, int num=-1);

    point& operator[](int); //Renvoi un point
    point operator[](int) const; //Renvoi un point

    int nb_triangles() const;
    real get_taille() const;
    int get_num() const;
    bool est_interieure() const;

    int& operator()(int); //Renvoi le num du triangle i
    int operator()(int) const; //Renvoi le num du triangle i

    void ajoute_triangle(int);
    void change_num(int);
    void change_interieur(bool);
    
    void enleve_triangle(int); 
};

bool operator==(const arete&, const arete&);
bool operator!=(const arete&, const arete&);
//bool est_meme_triangle(const arete&, const arete& , int &);
#endif