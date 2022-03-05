#include "../header/geom_avancee.hpp"
#include "../header/matrice.hpp"


polygone boite_circonscrite(const polygone& pol)
{
    polygone rect;
    double xmin, xmax, ymin, ymax;
    xmin=rect[0][0];
    xmax=rect[0][0];
    ymin=rect[0][1];
    ymax=rect[0][1];

    for(int i=1; i<rect.size(); i++)
    {
        if( pol[i][0]<xmin)
        {xmin=rect[i][0];}

        if( pol[i][0]>xmax)
        {xmax=rect[i][0];}
        
        if( pol[i][0]<xmin)
        {xmin=rect[i][0];}
        
        if( pol[i][0]>xmax)
        {xmax=rect[i][0];}
    }

    rect.ajoute_point( point(xmin,ymax) );
    rect.ajoute_point( point(xmax,ymax) );
    rect.ajoute_point( point(xmax,ymin) );
    rect.ajoute_point( point(xmin,ymin) );

    return rect;
}

bool estDedans(const point& x, const polygone& pol)
{
    double somme(0.0);
    int n=pol.size();
    double angle(1.0);

    for(int i=0; i<n; i++)
    {
        angle = std::acos( produit_scalaire( pol[i] - x, pol[(i+1)%n]-x ) / ( longueur_points(pol[i],x)*longueur_points(pol[(i+1)%n],x) ) );

        if(produit_scalaire( pol[i] - x , (pol[(i+1)%n]-x).conjugue() )< 0 )
        {
            angle*=-1;
        }
        somme+=angle;
    }

    somme /= 2*3.1415;
    //std::cout<<somme<<std::endl;

    if(somme-0.5>=0.0)
    {return true;}

    return false;
}

point normale_sortante(const triangle &tr, const arete &a)
{
    for(int n_oppose=0; n_oppose<3; n_oppose++)
    {
        if(tr[n_oppose]!=a[0] && tr[n_oppose]!=a[1])
        {
            droite mediane( point( -tr[(n_oppose+1)%3][1] + tr[(n_oppose+2)%3][1] , tr[(n_oppose+1)%3][0]-tr[(n_oppose+2)%3][0] ) , 0.5*(tr[(n_oppose+1)%3]+tr[(n_oppose+2)%3]) );
            if( produit_scalaire( mediane.get_vdirecteur(), mediane.get_origine()-tr[n_oppose] ) > 0 )
            {
                return mediane.get_vdirecteur()/mediane.get_vdirecteur().norme2();
            }
            else
            {
                return (-1)*( mediane.get_vdirecteur()/mediane.get_vdirecteur().norme2() );
            }
        }
    }
    std::cout<<"mauvaise normale normale_sortante"<<std::endl;
    exit(0);
}

/////////////CLASSE DROITE////////////////////////


droite::droite(const point &vec_directeur,const point &origine)
:m_vecteur_directeur(vec_directeur), m_origine(origine)
{
    if(vec_directeur==point(0,0))
    {
        std::cout<<"Un vecteur directeur nul ne permet pas de définir une droite"<<std::endl;
        exit(0);
    }
}

point droite::get_origine()
{
    return m_origine;
}

point droite::get_vdirecteur()
{
    return m_vecteur_directeur;
}


point droite::get_point(double t)
{
    return m_origine + t*m_vecteur_directeur;
}

void droite::draw(sf::RenderWindow& w,double taille)
{
    point debut = get_point(-0.5*taille);
    point fin = get_point(0.5*taille);
    sf::VertexArray line(sf::LineStrip, 2);
    line[0].position = sf::Vector2f(debut[0],debut[1]);
    line[1].position = sf::Vector2f(fin[0],fin[1]);

    line[0].color= sf::Color::Blue;
    line[1].color= sf::Color::Red;

    w.draw(line);
}



//////////////////////////////////////////////////


point intersection_droites(droite& d1, droite& d2)
{
    if(d1.get_origine()==d2.get_origine())
    {
        return d1.get_origine();
    }
    
    matrice<double> A(2,2);
    matrice<double> B(2,1);
    A(1,1) =   (d1.get_vdirecteur())[0];
    A(2,1) =   (d1.get_vdirecteur())[1];
    A(1,2) =   -(d2.get_vdirecteur())[0];
    A(2,2) =   -(d2.get_vdirecteur())[1];

    B(1,1) =  (d2.get_origine())[0]-(d1.get_origine())[0] ;
    B(2,1) =  (d2.get_origine())[1]-(d1.get_origine())[1] ;

    matrice<double> Ai(inverse(A));
    matrice<double> X(Ai*B);

    return ( d1.get_origine() + X(1,1)*d1.get_vdirecteur() );
}


