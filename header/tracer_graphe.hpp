#ifndef TRACER_GRAPHE_HPP
#define TRACER_GRAPHE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


template<typename T> void tracer(const std::vector<T> &X, const std::vector<T> &Y, sf::RenderWindow &w)
{
    if(X.empty() || Y.empty() || X.size()!=Y.size() )
    {
        std::cout<<"tracer mauvaise dimension"<<std::endl;
        exit(0);
    }
    //On trouve l'échelle
    T xmin=X[0];
    T xmax=X[0];
    T ymin=Y[0];
    T ymax=Y[0];

    int H=w.getSize().x;
    int L=w.getSize().y;

    for(int i=1; i<X.size(); i++)
    {
        if(X[i]<xmin)
            xmin=X[i];
        if(X[i]>xmax)
            xmax=X[i];
        if(Y[i]<ymin)
            ymin=Y[i];
        if(Y[i]>xmax)
            ymax=Y[i];
    }

    sf::VertexArray graphe(sf::LineStrip, X.size());

    for(int i=0; i<X.size(); i++ )
    {
        graphe[i].position = sf::Vector2f( L*(X[i]-xmin)/(xmax-xmin) , H*(1-(Y[i]-ymin)/(ymax-ymin) ) );  
        graphe[i].color = sf::Color::White;    
    }

    w.draw(graphe);
}


#endif