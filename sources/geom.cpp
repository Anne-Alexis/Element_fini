#include "../header/geom.hpp"
#include "../header/geom_avancee.hpp"

////POINT///////
point::point(real x, real y) 
: m_x(x), m_y(y) 
{ }

real& point::operator[](unsigned int i)
{
    if (i==0)
    {   
        return m_x;
    } 
    else if(i==1)
    {
        return m_y;
    }
    std::cout<<"Mauvaise numérotation [] point" <<std::endl;
    exit(0);
}

real point::operator[](unsigned int i) const
{
    if (i==0)
    {   
        return m_x;
    }
    else if(i==1)
    {
        return m_y;
    }
    std::cout<<"Mauvaise numérotation [] point" <<std::endl;
    exit(0);  
}

    //INTERNES
point& point::operator-=(const point& b)
{
    m_x -= b[0] ;
    m_y -= b[1] ;
    return *this;
}

point& point::operator+=(const point& b)
{
    m_x+=b[0];
    m_y+=b[1];
    return *this;
}

point& point::operator*=(real x)
{
    m_x*=x;    
    m_y*=x;
    return *this;
}

point& point::operator/=(real x)
{
    m_x/=x;    
    m_y/=x;
    return *this;
}

    //EXTERNES
point operator-(const point& a,const point& b)
{
    point res=a;
    return res-=b;      
}

point operator+(const point& a,const point& b)
{
    point res=a;
    return res+=b;      
}

point operator*(real x, const point & b)
{
    point res=b;
    return res*=x;
}
point operator*(const point & b, real x)
{
    point res=b;
    return res*=x;
}
point operator/(real x, const point & b)
{
    point res=b;
    return res/=x;
}
point operator/(const point & b, real x)
{
    point res=b;
    return res/=x;
}

real operator^(const point &a, const point &b)
{
    return a[0]*b[1]-a[1]*b[0];
}

point operator/(const point &a, const point &b) //division terme à terme
{
    point res= a;
    res[0]/=b[0];
    res[1]/=b[1];
    return res;
}

bool operator==(const point &a,const point &b)
{
    if(a[0]==b[0] && a[1]==b[1])
        {return true;}
    return false;
}

bool operator!=(const point &a, const point &b)
{
    return !(a==b);
}


    ////////////////

point& point::transformation_affine(const triangle & ref)
{
    real x = m_x;
    real y = m_y;
    this->m_x = ref[0][0] + (ref[1][0]-ref[0][0])*x + (ref[2][0]-ref[0][0])*y ;
    this->m_y = ref[0][1] + (ref[1][1]-ref[0][1])*x + (ref[2][1]-ref[0][1])*y ;
    return *this;
}


point point::conjugue() const
{
    point conj(m_y,-m_x);
    return conj;
}

real point::norme2() const
{
    return std::sqrt( m_x*m_x + m_y*m_y );
}

////////POLYGONE//////////

polygone::polygone()
{
    std::vector<point> sommets;
    sommets.clear();
    m_sommets=sommets;
}

polygone::polygone(const std::vector<point>& pts)
: m_sommets(pts)
{}

std::vector<point> polygone::get_sommets() const
{ return m_sommets; }

point& polygone::operator[](int i)
{ return m_sommets[i]; }

point polygone::operator[](int i) const
{ return m_sommets[i]; }

point& polygone::operator()(int i)
{ return m_sommets[i-1]; }

point polygone::operator()(int i) const
{ return m_sommets[i-1]; }

polygone& polygone::operator*=(real x)
{
    for(int i=0; i<m_sommets.size(); i++)
    {
        m_sommets[i]*=x;
    }
    return *this;
}

int polygone::trouve_oreille() const
{
    //Trouver la meilleure oreille
    real meilleure_qualite(0.0);
    int meilleur_sommet;

    std::vector<point> pts_or;
    pts_or.clear();
    pts_or.push_back(m_sommets[0]);
    pts_or.push_back(m_sommets[1]);
    pts_or.push_back(m_sommets[2]);
    triangle ore(pts_or);
    

    int n=m_sommets.size();
    bool admissible=true;

    for(int i=0; i<n; i++)
    {
        //test si l'oreille est admissible, i.e il n'y pas de sommet dans le triangle
        ore[0]=m_sommets[(i-1)%n ];
        ore[1]=m_sommets[i];
        ore[2]=m_sommets[(i+1)%n ];

        admissible=true;
        
        for(int j=0; j<n; j++)
        {
            if ( j!=i  &&  j!=(i-1)%n  &&  j!=(i+1)%n )
            {
                if(estDedans(m_sommets[j],ore))
                {
                    admissible=false;
                }
            }
        }

        if(admissible)
        {
            //On vérifie d'abord que le triangle est dans le polygone:
            if( estDedans(0.5*(m_sommets[(i+1)%n] + m_sommets[(i-1)%n ]) , *this) )
            {
                if (ore.qualite() > meilleure_qualite)
                {
                    meilleure_qualite=ore.qualite();
                    meilleur_sommet=i;
                }
            }
        }

    }

    return meilleur_sommet;

}


void polygone::remove(int i)
{
    m_sommets.erase(m_sommets.begin()+i);
}


void polygone::ajoute_point(point p)
{
    m_sommets.push_back(p);
}

int polygone::size() const
{
    return m_sommets.size();
}

void polygone::draw(sf::RenderWindow& w) const
{
    int n=m_sommets.size();
    sf::VertexArray lines(sf::LineStrip, n+1);

    for(int i=0; i<n; i++)
    {
        lines[i].position = sf::Vector2f( m_sommets[i][0], m_sommets[i][1]);
        lines[i].color= sf::Color::Green;
    }

    lines[n].position= sf::Vector2f( m_sommets[0][0], m_sommets[0][1]);
    lines[n].color= sf::Color::Green;

    w.draw(lines);
}

bool operator==(const polygone &P, const polygone&Q)
{
    if( P.get_sommets()==Q.get_sommets() )
    {
        return true;
    }
    return false;
}

bool operator!=(const polygone &P, const polygone&Q)
{
    return !(P==Q);
}

////TRIANGLE//////

triangle::triangle(const std::vector<point> & sommets)
:polygone(sommets)
{
    if(sommets.size() != 3 )
    {
        throw("Vous avez créer un triangle avec autre chose que trois sommets");
    }
}


triangle& triangle::transformation_affine(triangle& ref)
{
    for(int i=0; i<3; i++)
    {
        m_sommets[i].transformation_affine(ref);
    }

    return *this;
}

real triangle::demi_perimetre() const
{
    real a = longueur_points(m_sommets[0],m_sommets[1]);
    real b = longueur_points(m_sommets[1],m_sommets[2]);
    real c = longueur_points(m_sommets[2],m_sommets[0]);

    return (a+b+c)/2.0;
}

real triangle::aire() const
{
    //On utilise la formule de Héron
    real a=longueur_points(m_sommets[0],m_sommets[1]);
    real b=longueur_points(m_sommets[1],m_sommets[2]);
    real c=longueur_points(m_sommets[2],m_sommets[0]);

    real  pk=this->demi_perimetre();

    return std::sqrt( pk*(pk-a)*(pk-b)*(pk-c) );
}


real triangle::rayon_cercle_inscrit() const
{
    real pk=this->demi_perimetre();
    real sk=this->aire();

    return sk/pk;
}

real triangle::rayon_cercle_circonscrit() const
{
    real a=longueur_points(m_sommets[0],m_sommets[1]);
    real b=longueur_points(m_sommets[1],m_sommets[2]);
    real c=longueur_points(m_sommets[2],m_sommets[0]);
    real sk=this->aire();
    
    return (a*b*c) / (4*sk);
}


real triangle::qualite() const
{
    return this->rayon_cercle_inscrit()/this->rayon_cercle_circonscrit();   
}


point triangle::centre_cercle_circonscrit() const
{
    droite mediane1( point( -m_sommets[1][1]+m_sommets[0][1] , m_sommets[1][0]-m_sommets[0][0] ) , 0.5*(m_sommets[0]+m_sommets[1]) );
    droite mediane2( point( -m_sommets[2][1]+m_sommets[1][1] , m_sommets[2][0]-m_sommets[1][0] ) , 0.5*(m_sommets[1]+m_sommets[2]) );

    return intersection_droites(mediane1, mediane2);
}

void triangle::drawmediane(sf::RenderWindow& w) const
{
    droite mediane1( point( -m_sommets[1][1]+m_sommets[0][1] , m_sommets[1][0]-m_sommets[0][0] ) , 0.5*(m_sommets[0]+m_sommets[1]) );
    droite mediane2( point( -m_sommets[2][1]+m_sommets[1][1] , m_sommets[2][0]-m_sommets[1][0] ) , 0.5*(m_sommets[1]+m_sommets[2]) );

    mediane1.draw(w,5);
    mediane2.draw(w,5);
}

void triangle::draw_fill(real xa, real xb, real xc, std::pair<real,real> echelle,sf::RenderWindow &w) const
{
    sf::VertexArray tr(sf::Triangles, 3);

    // on définit la position des sommets du triangle
    tr[0].position = sf::Vector2f(m_sommets[0][0], m_sommets[0][1]);
    tr[1].position = sf::Vector2f(m_sommets[1][0], m_sommets[1][1]);
    tr[2].position = sf::Vector2f(m_sommets[2][0], m_sommets[2][1]);

    // on définit la couleur des sommets du triangle
    tr[0].color = rvb(xa, echelle.first, echelle.second);
    tr[1].color = rvb(xb, echelle.first, echelle.second);
    tr[2].color = rvb(xc, echelle.first, echelle.second);

    w.draw(tr);
}


point triangle::centre_cercle_inscrit() const
{
    std::cout<<"La fonction centre_cercle_inscrit n'as pas été codée"<<std::endl;
    return m_sommets[0];
}


///FONCTIONS////

real longueur_points(const point & a, const point & b)
{
    point x = a - b;
    real res= produit_scalaire(x,x) ;

    return std::sqrt(res);
}

real produit_scalaire(const point &a ,const point &b)
{
    return a[0]*b[0] + a[1]*b[1];
}

