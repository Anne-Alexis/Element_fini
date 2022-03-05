#ifndef FEM_NC_HPP
#define FEM_NC_HPP

#include "matrice.hpp"
#include "maillage.hpp"
#include "parametres.hpp"
#include <cmath>
#include <string>


matrice<real> reference(const maillage &);
void affichage_solution_nc(const matrice<real> &, const maillage &, sf::RenderWindow &);


matrice<real> matrice_rigidite_nc(const maillage &);

matrice<real> second_membre_nc(const matrice<real> &f,const maillage &);
matrice<real> source(const maillage &);
matrice<real> source_nulle(const maillage &);

matrice<real> resolution_nc(const matrice<real> &, const matrice<real> &, const maillage &);

matrice<real> dirichlet_q8(const maillage &);

//gestion de l'échelle
std::pair<real,real> trouve_echelle(const matrice<real> &S);
void affichage_echelle(const std::pair<real, real> &, sf::RenderWindow &);

real erreur_nc_q5(const matrice<real> &, const maillage &);

#endif