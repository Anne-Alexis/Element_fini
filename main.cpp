#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>

#include <vector>
#include "header/tracer_graphe.hpp"
#include "header/matrice.hpp"
#include "header/geom.hpp"
#include "header/maillage.hpp"
#include "header/matrice_creuse.hpp"

#include "header/fem_nc.hpp"
#include "header/parametres.hpp"
#include "header/polynome.hpp"

using namespace std;

//Test de la résolution directe en utilisant l'algorithme de widemann 
//avec des matrices creuses

int main()
{
    //Test des polynomes
    std::vector<real> V={1,2,3,4,5,6,7,8,9,10};
    polynome<real> P(V);
    cout<<"degre :" << P.get_degre()<<endl;
    cout<<P[0]<<endl;
    cout<<P[1]<<endl;
    cout<<P[9]<<endl;
    //cout<<P[11]<<endl;
    cout<<P<<endl;
    cout<<P+P<<endl;
    cout<<P*P<<endl;
    cout<<P<<endl;

    P*=2;
    cout<<P<<endl;
    P/=3;
    cout<<P<<endl;
    P.ajoute_coeff(0.4);
    cout<<P<<endl;

    cout<<"Essai polynomes de tailles différentes"<<endl;
    polynome<real> Q(std::vector<real> {-1,-2,-3,-4});
    cout<<P+Q<<endl;
    cout<<P-Q<<endl;
    cout<<P*Q<<endl;

    /*
    sf::RenderWindow window(sf::VideoMode(600, 600), "Maillage");

    std::vector<point> sommets_poly;
    sommets_poly.clear();

    sommets_poly.push_back( point( 0.0, 0.0 ) );
    sommets_poly.push_back( point( 1.0, 0.0 ) );
    sommets_poly.push_back( point( 1.0, 1.0 ) );
    sommets_poly.push_back( point( 0.0, 1.0 ) );


    cout<<"Création du maillage"<<endl;
    polygone pl (sommets_poly);
    maillage mai(pl, 30.0);

    //mai.ameliorer_delaunay();
    mai.raffinage_par2();
    mai.raffinage_par2();
    mai.raffinage_par2();

    cout<<mai.nb_triangles()<<endl;
    cout<<mai.nb_aretes()<<endl;
    cout<<mai.nb_aretes_interieures()<<endl;
    cout<<mai.nb_sommets()<<endl;
    

    //EX1 Q4
    cout<<"Initialisation"<<endl;
    matrice<real> K( matrice_rigidite_nc(mai) );
    matrice<real> f( source(mai) );
    matrice<real> B( second_membre_nc(f,mai));
    cout<<"Résolution directe"<<endl;
    matrice<real> U (resolution_nc(K,B,mai));
    //matrice U = reference(mai);



    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();
        
        affichage_solution_nc(U,mai,window);
        //mai.draw(window);


        window.display();
    }
    */

    return 0;
}