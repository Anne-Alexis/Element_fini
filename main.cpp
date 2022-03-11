#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>

#include <vector>
#include <utility>
#include "header/tracer_graphe.hpp"
#include "header/matrice.hpp"
#include "header/geom.hpp"
#include "header/maillage.hpp"
#include "header/matrice_creuse.hpp"

#include "header/fem_nc.hpp"
#include "header/parametres.hpp"
#include "header/arithmetique.hpp"

using namespace std;

//Test de la résolution directe en utilisant l'algorithme de widemann 
//avec des matrices creuses

int main()
{
    //Test de Berlekamp-Massey

    /*
    std::vector<real> S={0.4, -0.8, 3.6, 1.2};
    polynome<real> C(berlekamp_massey(S));
    cout<<"Résultat"<<endl;
    cout<<C<<endl;
    */

    /*
    entier a=120;
    entier b=23;

    cout<<euclide_etendu(a,b)[0]<<endl;
    cout<<euclide_etendu(a,b)[1]<<endl;
    cout<<euclide_etendu(a,b)[2]<<endl;
    */

   /*
   polynome<real> P({0,-2,3,-1,-1,1});
   polynome<real> Q({1,-1,1});

   pair< polynome<real>, polynome<real> > div( div_euclide(P,Q) ) ;

   cout<<div.first<<endl;
   cout<<div.second<<endl;
   */

    //polynome<real> P({1,-2,1});
    //polynome<real> Q({-1,1});
    /*
    polynome<real> P({0,-2,3,-1,-1,1});
    polynome<real> Q({1,-1,1});

    euclide_etendu(P,Q);

    //euclide_etendu(P,Q)[0]<<endl;

    polynome<real> U=euclide_etendu(P,Q)[1];
    polynome<real> V=euclide_etendu(P,Q)[2];

    std::cout<<U<<endl;
    std::cout<<V<<endl;

    cout<<U*P+V*Q<< endl;
    */

    vector<real> s({-2929918,-347154,-40039,-3898,21,222,134,12});
    polynome<real> C(berlekamp_massey(s));
    cout<<C<<endl;

    return 0;
}