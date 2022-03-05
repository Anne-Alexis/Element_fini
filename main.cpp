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

using namespace std;

//ameliorer_delaunay ne marche pas

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Maillage");


    std::vector<point> sommets_poly;
    sommets_poly.clear();

    // sommets_poly.push_back( point(100.0,100.0) );
    // sommets_poly.push_back( point(200.0,70.0) );
    // sommets_poly.push_back( point(300.0,140.0) );
    // sommets_poly.push_back( point(400.0,100.0) );
    // sommets_poly.push_back( point(400.0,400.0) );
    // sommets_poly.push_back( point(180.0,400.0) );
    // sommets_poly.push_back( point(180.0,300.0) );
    // sommets_poly.push_back( point(150.0,300.0) );
    // sommets_poly.push_back( point(150.0,450.0) );
    // sommets_poly.push_back( point(200.0,450.0) );
    // sommets_poly.push_back( point(200.0,480.0) );
    // sommets_poly.push_back( point(130.0,480.0) );
    // sommets_poly.push_back( point(100.0,400.0) );


    //EX1 Q4
    sommets_poly.push_back( point( 0.0, 0.0 ) );
    sommets_poly.push_back( point( 1.0, 0.0 ) );
    sommets_poly.push_back( point( 1.0, 1.0 ) );
    sommets_poly.push_back( point( 0.0, 1.0 ) );


    /*
    //EX1 Q8
    sommets_poly.push_back( point( 0.0, 0.0 ) );
    sommets_poly.push_back( point( 0.5, 0.0 ) );
    sommets_poly.push_back( point( 0.5, 0.5 ) );
    sommets_poly.push_back( point( 1.0, 0.5 ) );
    sommets_poly.push_back( point( 1.0, 1.0 ) );
    sommets_poly.push_back( point( 0.0, 1.0 ) );
    */


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

    /*
    //EX1 Q8
    cout<<"Initialisation"<<endl;
    matrice<real> K( matrice_rigidite_nc(mai) );
    matrice<real> f( source_nulle(mai) );
    matrice<real> B( second_membre_nc(f,mai));
    cout<<"Résolution directe"<<endl;
    matrice<real> U0 (resolution_nc(K,B,mai));
    cout<<"Reconstruction de la solution totale"<<endl;
    matrice<real> U( dirichlet_q8(mai) + U0);
    //matrice U = reference(mai);
    */


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

        //tracer(X,Y,window);


        window.display();
    }

    return 0;
}