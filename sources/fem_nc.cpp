#include "../header/fem_nc.hpp"
 

matrice<real> reference(const maillage &mai)
{
    int N=mai.nb_aretes();
    matrice<real> S(N,1);
    for(int i=0; i<N ;i++)
    {
        point xf= 0.5*( mai.get_arete(i)[0] + mai.get_arete(i)[1] );
        S(i+1,1) = std::sin(PI*xf[0])*std::sin(PI*xf[1]);
    }
    return S;
}

matrice<real> source_nulle(const maillage & mai)
{
    int N=mai.nb_aretes();
    matrice<real> S(N,1);
    for(int i=1; i<=N ;i++)
    {
        S(i,1) = 0;
    }
    return S;
}

matrice<real> matrice_rigidite_nc(const maillage & mai)
{
    int nb_aretes= mai.nb_aretes();
    matrice<real> M(mai.nb_aretes_interieures(),mai.nb_aretes_interieures());
    int a=1; int b=1;

    for(int i=0; i<nb_aretes ; i++)
    {
        if(mai.get_arete(i).est_interieure())
        {
            real lg2=( mai.get_arete(i).get_taille() )*( mai.get_arete(i).get_taille() );
            M(a,a)=0;
            for(int k=0; k<mai.get_arete(i).nb_triangles() ; k++)
            {
                M(a,a) += lg2/mai.get_triangle(mai.get_arete(i)(k)).aire();
            }
            b=a+1;
            for(int j=i+1; j<nb_aretes ;j++)
            {
                if (mai.get_arete(j).est_interieure() )
                {
                    int num_tri_commun;
                    if ( mai.arete_meme_triangle( i, j, num_tri_commun ) )
                    {
                        M(a,b) = ( mai.get_arete(i).get_taille() )*( mai.get_arete(j).get_taille() ) / ( mai.get_triangle(num_tri_commun).aire() );
                        M(a,b) *= produit_scalaire( normale_sortante( mai.get_triangle(num_tri_commun), mai.get_arete(i) ), normale_sortante(mai.get_triangle(num_tri_commun), mai.get_arete(j)) );
                    }
                    else
                    {
                        M(a,b) = 0;
                    }
                    M(b,a)=M(a,b);
                    b++;
                }
            }
            a++;    
        }
    }
    return M; 
}


matrice<real> second_membre_nc(const matrice<real> &f,const maillage & mai)
{
    matrice<real> res(mai.nb_aretes_interieures(),1);

    int a=1;
    for(int i=0; i<mai.nb_aretes() ;i++)
    {
        if(mai.get_arete(i).est_interieure())
        {
            res(a,1)=0;
            for(int k=0; k< mai.get_arete(i).nb_triangles(); k++ )
            {
                res(a,1) += f(a,1)*mai.get_triangle( mai.get_arete(i)(k) ).aire();
            }
            res(a,1)/=3;
            a++;
        }
    } 
    return res;
}


matrice<real> source(const maillage &mai)
{
    int nb_aretes= mai.nb_aretes();
    matrice<real> f(mai.nb_aretes_interieures(),1);

    int a=1;
    for(int i=0; i<nb_aretes; i++)
    {
        if( mai.get_arete(i).est_interieure())
        {
            point xf= 0.5*( mai.get_arete(i)[0] + mai.get_arete(i)[1] );
            f(a,1)= 2*PI*PI*std::sin(PI*xf[0])*std::sin(PI*xf[1]);
            a++;
        }
    }
    return f;
}


matrice<real> resolution_nc(const matrice<real> &K,const matrice<real> &B, const maillage &mai)
{
    int N=mai.nb_aretes();
    matrice<real> S(N,1);
    matrice<real> U(inverse(K)*B);

    int k(1);

    for(int i=0; i<N; i++)
    {
        if(mai.get_arete(i).est_interieure())
        {
            S(i+1,1)=U(k,1);
            k++;   
        }
        else
        {
            S(i+1,1)=0;
        }
    }
    return S;
}

matrice<real> dirichlet_q8(const maillage & mai)
{
    int N=mai.nb_aretes();
    matrice<real> res(N,1);
    point c(0.5,0.5);
    for(int i=0; i<N; i++)
    {
        point xf= 0.5*(mai.get_arete(i)[0]+mai.get_arete(i)[1]);
        real r = (xf-c).norme2() ;
        real x=(xf[0]-c[0]);
        real y=(xf[1]-c[1]);
        real ang=std::atan( y/std::abs(x) );

        res(i+1,1)= std::pow(r,2.0/3.0)*std::sin( ang*2.0/3.0);
    }
    return res;
}


void affichage_solution_nc(const matrice<real> &S, const maillage &mai, sf::RenderWindow &w)
{
    int N=mai.nb_aretes();

    if( N!=S.nb_lignes() || S.nb_colonnes()!= 1)
    {
        std::cout<<"Affichage matrice de dimension incompatible avec le maillage"<<std::endl;
    }
    else
    {
        std::pair<real, real> echelle=trouve_echelle(S);

        for(int i=0; i<mai.nb_triangles() ; i++)
        {
            point cab= 0.5*( mai.get_arete( mai.get_triangle(i).get_narete(0) )[0] + mai.get_arete( mai.get_triangle(i).get_narete(0) )[1] );
            point cbc= 0.5*( mai.get_arete( mai.get_triangle(i).get_narete(1) )[0] + mai.get_arete( mai.get_triangle(i).get_narete(1) )[1] );
            point cca= 0.5*( mai.get_arete( mai.get_triangle(i).get_narete(2) )[0] + mai.get_arete( mai.get_triangle(i).get_narete(2) )[1] );

            real vab= S[mai.get_triangle(i).get_narete(0)];
            real vbc= S[mai.get_triangle(i).get_narete(1)];
            real vca= S[mai.get_triangle(i).get_narete(2)];

            matrice<real> A( 3,3 ,std::vector<real>({cab[0],cab[1],1, cbc[0],cbc[1],1, cca[0],cca[1],1}) );
            matrice<real> B( 3,1, std::vector<real>({vab,vbc,vca}));

            matrice<real> IT(inverse(A)*B);

            real xa= IT(1,1)*mai.get_sommet( mai.get_triangle(i).get_nsommet(0) )[0] + IT(2,1)*mai.get_sommet( mai.get_triangle(i).get_nsommet(0) )[1] + IT(3,1);
            real xb= IT(1,1)*mai.get_sommet( mai.get_triangle(i).get_nsommet(1) )[0] + IT(2,1)*mai.get_sommet( mai.get_triangle(i).get_nsommet(1) )[1] + IT(3,1);
            real xc= IT(1,1)*mai.get_sommet( mai.get_triangle(i).get_nsommet(2) )[0] + IT(2,1)*mai.get_sommet( mai.get_triangle(i).get_nsommet(2) )[1] + IT(3,1);

            triangle t(mai.get_triangle(i));
            t*=500;
            t.draw_fill(xa,xb,xc,echelle,w);
        }

        affichage_echelle(echelle,w);
    }
}

std::pair<real,real> trouve_echelle(const matrice<real> &S)
{
    real min=S(1,1);
    real max=S(1,1);

    for(int i=1; i<=S.nb_lignes(); i++)
    {
        for(int j=1; j<=S.nb_colonnes(); j++)
        {
            if(S(i,j)>max)
                max=S(i,j);

            if(S(i,j)<min)
                min=S(i,j);
        }
    }
    return std::make_pair(min,max);
}

void affichage_echelle(const std::pair<real, real> &echelle, sf::RenderWindow &w)
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout<<"Chargement arial impossible"<<std::endl;
    }

    sf::VertexArray rec(sf::Quads, 4);

    // on définit la position des sommets du triangle   
    rec[0].position = sf::Vector2f(525, 100);
    rec[1].position = sf::Vector2f(575, 100);
    rec[2].position = sf::Vector2f(575, 400);
    rec[3].position = sf::Vector2f(525, 400);

    // on définit la couleur des sommets du triangle
    rec[0].color = sf::Color::White;
    rec[1].color = sf::Color::White;
    rec[2].color = sf::Color::Black;   
    rec[3].color = sf::Color::Black;  

    sf::Text min;
    sf::Text max;
    min.setString( std::to_string(echelle.first) );
    max.setString( std::to_string(echelle.second) );

    max.setPosition(525,80);
    min.setPosition(525,408);
    min.setCharacterSize(15);
    max.setCharacterSize(15);
    min.setFont(font);
    max.setFont(font);

    w.draw(rec);
    w.draw(min);
    w.draw(max);
}

real erreur_nc_q5(const matrice<real> & U, const maillage &mai)
{
    real err_tot(0);
    for(int k=0; k<mai.nb_triangles(); k++)
    {
        real K=mai.get_triangle(k).aire();
        real err_K(0);
        for(int i=0; i<mai.get_triangle(k).nb_aretes(); i++)
        {
            int n_f( mai.get_triangle(k).get_narete(i) );
            point xf= 0.5*( mai.get_arete(n_f)[0] + mai.get_arete(n_f)[1] );
            point gradu( PI*std::cos(PI*xf[0])*std::sin(PI*xf[1]) , PI*std::sin(PI*xf[0])*std::cos(PI*xf[1]) );
            point graduh(0.0,0.0);
            for(int j=0; j< mai.get_triangle(k).nb_aretes(); j++)
            {
                int n_fj=mai.get_triangle(k).get_narete(j);
                if( mai.get_arete(n_fj).est_interieure() )
                {
                    real F=mai.get_arete(n_fj).get_taille();
                    graduh+= F*U( n_fj+1 ,1)*normale_sortante(mai.get_triangle(k),mai.get_arete(n_fj));            
                }
            }
            graduh/=K;
            err_K += (gradu-graduh).norme2();
        }
        err_tot += (K/3.0)*err_K;
    }

    return std::sqrt(err_tot);
}
