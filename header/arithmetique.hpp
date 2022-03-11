#ifndef ARITHMETIQUE_HPP
#define ARITHMETIQUE_HPP


#include <vector>
#include "polynome.hpp"

#define entier int
#define real double


    entier euclide(entier, entier); //Algo d'euclide classique pour calculer le pgcd
    std::vector<entier> euclide_etendu(entier, entier);

    polynome<real> euclide(const polynome<real> &, const polynome<real> &);
    std::vector< polynome<real> > euclide_etendu(const polynome<real> &, const polynome<real> &);

    polynome<real> pade_n(const polynome<real> &);




#endif