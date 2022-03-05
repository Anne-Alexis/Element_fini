#include "../header/maillage.hpp"



////////////////////////////////////MAILLAGE/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

///////////////CONSTRUCTEURS/////////////////////

maillage::maillage(polygone poly, real h)
: m_h(h)
{
    //On initialise le maillage sans triangle avec juste les sommets du polygone
    m_sommets.clear();
    m_triangles.clear();
    m_aretes.clear();
    std::vector<point> somm = poly.get_sommets();

    //On ajoute les sommets du polygone au maillage
    for(int i=0; i<somm.size(); i++)
    {
        m_sommets.push_back( sommet(somm[i], i) );
    }

    //Nombre de polynome
    int n=somm.size();
    //Numero de l'oreille
    int sommet_oreille;
    
    //Initialisation du vecteur contenant les points d'une oreille
    std::vector<point> pts_oreille;
    pts_oreille.clear();
    pts_oreille.push_back(poly[0]);
    pts_oreille.push_back(poly[1]);
    pts_oreille.push_back(poly[2]);

    //On boucle tant que le polynome reste plus grand qu'un triangle
    while (n>3)
    {
        sommet_oreille = poly.trouve_oreille();
        pts_oreille[0]=poly[ (sommet_oreille-1)%n ];
        pts_oreille[1]=poly[ sommet_oreille ];
        pts_oreille[2]=poly[ (sommet_oreille+1)%n ];
        this->ajoute_triangle( triangle(pts_oreille) );
        poly.remove(sommet_oreille);
        n-=1;
    }

    //La dernière oreille est le pylogone restant
    pts_oreille=poly.get_sommets();
    triangle tri(pts_oreille);
    this->ajoute_triangle(tri);

    //Une fois qu'on a les oreilles divise et finie le maillage
    //this->ameliorer_delaunay();
    //this->raffinage_par2();

    this->ajuste_aretes_interieures();
}

///////////////ACCESSEURS////////////////////

sommet& maillage::get_sommet(int i)
{
    if(i<0 || i>m_sommets.size() )
    {
        std::cout<<"Indexation maillage::get_sommet"<<std::endl;
        exit(0);
    }
    return m_sommets[i];
}

sommet maillage::get_sommet(int i) const
{
    if(i<0 || i>m_sommets.size() )
    {
        std::cout<<"Indexation maillage::get_sommet const"<<std::endl;
        exit(0);
    }
    return m_sommets[i];
}

maille_triangle& maillage::get_triangle(int i)
{
    if(i<0 || i>m_triangles.size() )
    {
        std::cout<<"Indexation maillage::get_triangle"<<std::endl;
        exit(0);
    }
    return m_triangles[i];
}

maille_triangle maillage::get_triangle(int i) const
{
    if(i<0 || i>m_triangles.size() )
    {
        std::cout<<"Indexation maillage::get_triangle const"<<std::endl;
        exit(0);
    }
    return m_triangles[i];
}

arete& maillage::get_arete(int i)
{
    if(i<0 || i>m_aretes.size() )
    {
        std::cout<<"Indexation maillage::get_arete"<<std::endl;
        exit(0);
    }
    return m_aretes[i];
}

arete maillage::get_arete(int i) const
{
    if(i<0 || i>m_aretes.size() )
    {
        std::cout<<"Indexation maillage::get_aretes const"<<std::endl;
        exit(0);
    }
    return m_aretes[i];
}

int maillage::nb_triangles() const
{
    return m_triangles.size();
}

int maillage::nb_sommets() const
{
    return m_sommets.size();
}

int maillage::nb_aretes() const
{
    return m_aretes.size();
}

int maillage::nb_aretes_interieures() const
{
    return m_nb_aretes_interieures;
}

////////MODIFICATEURS "SIMPLES"///////

bool maillage::ajoute_triangle(const triangle& tr, int num_tri, std::vector<int> num_pts, std::vector<int> num_aretes )
{
    int N(num_tri);

    if( N==-1 )
    {
        N=m_triangles.size();
        //On vérifie que le triangle n'existe pas déjà
        for(int i=0; i<N; i++)
        {
            if(tr==m_triangles[i])
            {
                std::cout<<"Triangle déjà présent"<<std::endl;
                return false;
            }
        }
        m_triangles.push_back( maille_triangle(tr,N) );
    }
    else
    {
        (m_triangles[N])[0]=tr[0];
        (m_triangles[N])[1]=tr[1];
        (m_triangles[N])[2]=tr[2];
    }

    m_triangles[N].vide();

    int num_a=-1   ; int num_b=-1   ; int num_c=-1   ;
    point a(tr[0]) ; point b(tr[1]) ; point c(tr[2]) ;

    if( num_pts==std::vector<int>({-1,-1,-1}) )
    {
        for(int j=0; j<m_sommets.size(); j++)
        {
            if( a==m_sommets[j] )
                num_a=j;
            if( b==m_sommets[j] )
                num_b=j;
            if( c==m_sommets[j] )
                num_c=j;
        }
        if( num_a==-1 )
        {
            int nb_sommets=m_sommets.size();
            num_a=nb_sommets;
            m_sommets.push_back( sommet(a[0],a[1], nb_sommets) );
        }
        if( num_b==-1 )
        {
            int nb_sommets=m_sommets.size();
            num_b=nb_sommets;
            m_sommets.push_back( sommet(b[0],b[1],nb_sommets) );
        }
        if( num_c==-1 )
        {
            int nb_sommets=m_sommets.size();
            num_c=nb_sommets;
            m_sommets.push_back( sommet(c[0],c[1],nb_sommets) );
        }
    }
    else
    {
        num_a=num_pts[0];
        num_b=num_pts[1];
        num_c=num_pts[2];
    }
    m_triangles[N].ajoute_sommet(num_a);
    m_triangles[N].ajoute_sommet(num_b);
    m_triangles[N].ajoute_sommet(num_c);

    m_sommets[num_a].ajoute_triangle(N);
    m_sommets[num_b].ajoute_triangle(N);
    m_sommets[num_c].ajoute_triangle(N);

    int num_ab=-1;
    int num_bc=-1;
    int num_ca=-1;
    if( num_aretes==std::vector<int>({-1,-1,-1}) )
    {
        arete ab(a, b);
        arete bc(b, c);
        arete ca(c, a);

        for(int j=0; j<m_aretes.size(); j++)
        {
            if( ab==m_aretes[j] )
                num_ab=j;
            if( bc==m_aretes[j] )
                num_bc=j;
            if( ca==m_aretes[j] )
                num_ca=j;
        }
        if(num_ab==-1)
        {
            int nb_aretes=m_aretes.size();
            num_ab=nb_aretes;
            m_aretes.push_back( arete( ab[0], ab[1], num_a, num_b, nb_aretes ) );
        }
        if(num_bc==-1)
        {
            int nb_aretes=m_aretes.size();
            num_bc=nb_aretes;
            m_aretes.push_back( arete( bc[0], bc[1], num_b, num_c, nb_aretes ) );
        }
        if(num_ca==-1)
        {
            int nb_aretes=m_aretes.size();
            num_ca=nb_aretes;
            m_aretes.push_back( arete( ca[0], ca[1], num_c, num_a, nb_aretes ) );
        }
    }
    else
    {
        num_ab=num_aretes[0];
        num_bc=num_aretes[1];
        num_ca=num_aretes[2];
    }
    m_triangles[N].ajoute_arete(num_ab);
    m_triangles[N].ajoute_arete(num_bc);
    m_triangles[N].ajoute_arete(num_ca);
    m_aretes[num_ab].ajoute_triangle(N);
    m_aretes[num_bc].ajoute_triangle(N);
    m_aretes[num_ca].ajoute_triangle(N);

    return true;
}

void maillage::vide_triangle(int i)
{
    int n;
    for(int k=0; k<m_triangles[i].nb_sommets(); k++)
    {
        n=m_triangles[i].get_nsommet(k);
        m_sommets[n].enleve_triangle(i);
    }

    for(int k=0; k<m_triangles[i].nb_aretes(); k++)
    {
        n=m_triangles[i].get_narete(k);
        m_sommets[n].enleve_triangle(i);
    }

    m_triangles[i].vide();
}

void maillage::enleve_triangle(int i)
{
    for(int k=0; k<m_aretes.size(); k++)
    {
        m_aretes[k].enleve_triangle(i);
        for(int j=0; j<m_aretes[k].nb_triangles(); j++)
        {
            if( m_aretes[k](j)> i )
                m_aretes[k](j)--;
        }
    }
    for(int k=0; k<m_sommets.size(); k++)
    {
        m_sommets[k].enleve_triangle(i);
        for(int j=0; j<m_sommets[k].nb_triangles(); j++)
        {
            if( m_sommets[k].get_ntriangle(j)> i )
                m_sommets[k].get_ntriangle(j)--;
        }
    }
    m_triangles.erase(m_triangles.begin() + i);
    for(int j=i; j<m_triangles.size(); j++)
        m_triangles[j].change_num(j);
}

void maillage::enleve_arete(int i)
{
    for(int k=0; k<m_triangles.size(); k++)
    {
        m_triangles[k].enleve_arete(i);
        for(int j=0; j<m_triangles[k].nb_aretes(); j++)
        {
            if( m_triangles[k].get_narete(j)> i )
                m_triangles[k].get_narete(j)--;
        }
    }
    for(int k=0; k<m_sommets.size(); k++)
    {
        m_sommets[k].enleve_arete(i);
        for(int j=0; j<m_sommets[k].nb_aretes(); j++)
        {
            if( m_sommets[k].get_narete(j)> i )
                m_sommets[k].get_narete(j)--;
        }
    }
    m_aretes.erase(m_aretes.begin() + i);
    for(int j=i; j<m_aretes.size(); j++)
        m_aretes[j].change_num(j);
}

void maillage::ajuste_aretes_interieures()
{
    m_nb_aretes_interieures=0;
    for(int i=0; i<m_aretes.size(); i++)
    {
        if(m_aretes[i].nb_triangles()>1)
        {   
            m_aretes[i].change_interieur(true);
            m_nb_aretes_interieures++;
        }
        else
        {
            m_aretes[i].change_interieur(false);
        }
    }
}
/////////////METHODES//////////////

void maillage::draw(sf::RenderWindow & w) const
{
    for(int i=0; i < m_triangles.size() ; i++)
    {
        (this->get_triangle(i)).draw(w);
    }
}

void maillage::ameliorer_delaunay()
{
    int NT=this->nb_triangles();
    point point_cible(0.0, 0.0);
    int num_pt_i, num_pt_j;

    for(int i=0; i< NT ;i++) // On améliore pour chaque triangle
    {
        //On cherche les triangles voisins pour voir s'ils respectent le critère de delaunay
        //Voisin=une arête en commun, cad deux pts communs
        for(int j=i+1; j<NT; j++)
        {
            if( arete_commune(i, j, num_pt_i, num_pt_j) )
            {
                if( !est_delaunay( m_triangles[i], m_triangles[j][num_pt_j] ) )
                {
                    maille_triangle ti(m_triangles[i]);
                    maille_triangle tj(m_triangles[j]);
                    //Réarrangement des triangles
                    //Avant de le faire il faut vérifier que les nouveaux triangles sont admissibles cad que la nouvelle arete est dans le polygone
                    if( estDedans( 0.5*( tj[num_pt_j] + ti[num_pt_i] ), ti ) || estDedans( 0.5*( tj[num_pt_j] + ti[num_pt_i] ), tj ) )
                    {
                        std::vector<point> nvi = { ti[num_pt_i], tj[num_pt_j], ti[(num_pt_i+1)%3] };
                        std::vector<point> nvj = { ti[num_pt_i], tj[num_pt_j], ti[(num_pt_i+2)%3] };
                        
                        this->vide_triangle(i);
                        this->vide_triangle(j);

                        if(num_pt_i==0)
                        {
                            this->enleve_arete( ti.get_narete(1) );
                        }                        
                        if(num_pt_i==1)
                        {
                            this->enleve_arete( ti.get_narete(2) );
                        }                        
                        if(num_pt_i==2)
                        {
                            this->enleve_arete( ti.get_narete(0) );
                        }                        
                    

                        this->ajoute_triangle(nvi, i);
                        this->ajoute_triangle(nvj, j);

                        std::cout<<"Modif"<<std::endl;
                    }
                }
            }
        }

    }
}

bool maillage::arete_commune(int nt1, int nt2, int &num_pt_i, int &num_pt_j) const
{
    if( nt1<0 || nt2<0 || nt1>= m_triangles.size() || nt2>=m_triangles.size() )
    {
        std::cout<<"Mauvaise numérotation triangles arete_commune"<<std::endl;
        return false;
    }

    //
    if( ( this->get_triangle(nt1) ).get_narete(0) == ( this->get_triangle(nt2) ).get_narete(0) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(2);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(2);
        return true;
    }
    if( ( this->get_triangle(nt1) ).get_narete(1) == ( this->get_triangle(nt2) ).get_narete(0) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(0);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(2);
        return true;
    }
    if( ( this->get_triangle(nt1) ).get_narete(2) == ( this->get_triangle(nt2) ).get_narete(0) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(1);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(2);
        return true;
    }
    //
    if( ( this->get_triangle(nt1) ).get_narete(0) == ( this->get_triangle(nt2) ).get_narete(1) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(2);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(0);
        return true;
    }
    if( ( this->get_triangle(nt1) ).get_narete(1) == ( this->get_triangle(nt2) ).get_narete(1) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(0);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(0);
        return true;
    }
    if( ( this->get_triangle(nt1) ).get_narete(2) == ( this->get_triangle(nt2) ).get_narete(1) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(1);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(0);
        return true;
    }
    //
    if( ( this->get_triangle(nt1) ).get_narete(0) == ( this->get_triangle(nt2) ).get_narete(2) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(2);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(1);
        return true;
    }
    if( ( this->get_triangle(nt1) ).get_narete(1) == ( this->get_triangle(nt2) ).get_narete(2) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(0);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(1);
        return true;
    }
    if( ( this->get_triangle(nt1) ).get_narete(2) == ( this->get_triangle(nt2) ).get_narete(2) )
    {
        num_pt_i= ( this->get_triangle(nt1) ).get_nsommet(1);
        num_pt_j= ( this->get_triangle(nt1) ).get_nsommet(1);
        return true;
    }

    return false;
}

void maillage::raffinage_par2()
{
    int N_tri(m_triangles.size());
    for(int k=0; k<N_tri ;k++)
    {
        this->raffinage_par2_tri(0);
    }

    //On enlève les aretes en trop
    for(int i=0; i<m_aretes.size(); i++)
    {
        if(m_aretes[i].nb_triangles()==0)
        {
            this->enleve_arete(i);
            i--;
        }
    }

    this->ajuste_aretes_interieures();
}

//protected
void maillage::raffinage_par2_tri(int nt)
{
    //On créer les points au milieu
    int a = m_triangles[nt].get_nsommet(0);
    int b = m_triangles[nt].get_nsommet(1);
    int c = m_triangles[nt].get_nsommet(2);

    point ab = 0.5*( m_sommets[a] + m_sommets[b] );
    point bc = 0.5*( m_sommets[b] + m_sommets[c] );
    point ca = 0.5*( m_sommets[c] + m_sommets[a] );

    this->enleve_triangle(nt); 

    //On ajoute les triangles et on lie les numéro des points aux numéros des triangles
    this->ajoute_triangle( triangle( std::vector<point>{m_sommets[a], ab, ca} ) );
    this->ajoute_triangle( triangle( std::vector<point>{m_sommets[b], bc, ab} ) );
    this->ajoute_triangle( triangle( std::vector<point>{m_sommets[c], ca, bc} ) );
    this->ajoute_triangle( triangle( std::vector<point>{ab, bc, ca} ) );
    
}

bool maillage::arete_meme_triangle(int a1, int a2, int& num_tri) const
{
    for(int i=0; i<m_aretes[a1].nb_triangles(); i++)
    {
        for(int j=0; j<m_aretes[a2].nb_triangles(); j++)
        {
            if( m_aretes[a1](i)==m_aretes[a2](j) )
            {
                num_tri=m_aretes[a1](i);
                return true;
            }
        }
    }
    return false;
}

bool est_delaunay(const triangle &t1, const point &pt)
{
    real rc=t1.rayon_cercle_circonscrit();
    point  cc=t1.centre_cercle_circonscrit();
    if( produit_scalaire(pt-cc,pt-cc) < rc*rc )
        return false;
    return true;
}


////////////////////////CLASSE MAILLE_TRIANGLE///////////////////////////
/////////////////////////////////////////////////////////////////////////

maille_triangle::maille_triangle(const std::vector<point> &pts, int num)
: triangle(pts), m_num(num)
{
    m_num_sommets.clear();
    m_num_aretes.clear();
}

maille_triangle::maille_triangle(const triangle &tr, int num)
: triangle(tr), m_num(num)
{
    m_num_sommets.clear();
    m_num_aretes.clear();
}

int maille_triangle::nb_sommets() const//Normalement renvoi 3
{
    return m_num_sommets.size();
}

int maille_triangle::nb_aretes() const//Normalement renvoi 3
{
    return m_num_aretes.size();
}

int maille_triangle::get_num() const
{
    return m_num;
}
        
int maille_triangle::get_nsommet(int i) const
{
    if(i<0 || i>m_num_sommets.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_sommet"<<std::endl;
        exit(0);
    }
    return m_num_sommets[i];
}

int& maille_triangle::get_nsommet(int i)
{
    if(i<0 || i>m_num_sommets.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_sommet"<<std::endl;
        exit(0);
    }
    return m_num_sommets[i];
}

int maille_triangle::get_narete(int i) const
{
    if(i<0 || i>m_num_aretes.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_sommet"<<std::endl;
        exit(0);
    }
    return m_num_aretes[i];
}

int& maille_triangle::get_narete(int i)
{
    if(i<0 || i>m_num_aretes.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_sommet"<<std::endl;
        exit(0);
    }
    return m_num_aretes[i];
}

void maille_triangle::ajoute_arete(int i)
{
    m_num_aretes.push_back(i);
}
void maille_triangle::ajoute_sommet(int i)
{
    m_num_sommets.push_back(i);
}

void maille_triangle::change_num(int i)
{
    m_num=i;
}

void maille_triangle::enleve_arete(int i)
{
    for(int k=0; k<m_num_aretes.size(); k++)
    {
        if(m_num_aretes[k]==i)
        {
            m_num_aretes.erase(m_num_aretes.begin()+k);
            k--;
        }
    }
}
void maille_triangle::enleve_sommet(int i)
{
    for(int k=0; k<m_num_sommets.size(); k++)
    {
        if(m_num_sommets[k]==i)
        {
            m_num_sommets.erase(m_num_sommets.begin()+k);
            k--;
        }
    }
}

void maille_triangle::vide()
{
    m_num_sommets.clear();
    m_num_aretes.clear();
}


//////////////////////////CLASSE SOMMMET///////////////////////////
///////////////////////////////////////////////////////////////////

sommet::sommet(real x, real y, int num)
:point(x,y), m_num(num)
{
    m_num_aretes.clear();
    m_num_triangles.clear();
}

sommet::sommet(const point &pt, int num)
:point(pt), m_num(num)
{
    m_num_aretes.clear();
    m_num_triangles.clear();
}

int sommet::nb_triangles() const
{
    return m_num_triangles.size();
}

int sommet::nb_aretes() const
{
    return m_num_aretes.size();
}

int sommet::get_num() const
{
    return m_num;
}
        
int sommet::get_ntriangle(int i) const
{
    if(i<0 || i>m_num_triangles.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_triangle"<<std::endl;
        exit(0);
    }
    return m_num_triangles[i];
}

int& sommet::get_ntriangle(int i)
{
    if(i<0 || i>m_num_triangles.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_triangle"<<std::endl;
        exit(0);
    }
    return m_num_triangles[i];
}

int sommet::get_narete(int i) const
{
    if(i<0 || i>m_num_aretes.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_arete"<<std::endl;
        exit(0);
    }
    return m_num_aretes[i];
}

int& sommet::get_narete(int i)
{
    if(i<0 || i>m_num_aretes.size())
    {
        std::cout<<"Mauvaise numérotation maillage_triangle num_arete"<<std::endl;
        exit(0);
    }
    return m_num_aretes[i];
}

void sommet::ajoute_arete(int i)
{
    m_num_aretes.push_back(i);
}

void sommet::ajoute_triangle(int i)
{
    m_num_triangles.push_back(i);
}

void sommet::change_num(int i)
{
    m_num=i;
}

void sommet::enleve_arete(int i)
{
    for(int k=0; k<m_num_aretes.size(); k++)
    {
        if(m_num_aretes[k]==i)
        {
            m_num_aretes.erase(m_num_aretes.begin()+k);
            k--;
        }
    }
}
void sommet::enleve_triangle(int i)
{
    for(int k=0; k<m_num_triangles.size(); k++)
    {
        if(m_num_triangles[k]==i)
        {
            m_num_triangles.erase(m_num_triangles.begin()+k);
            k--;
        }
    }
}

/////////////////////////Classe arête/////////////////////////////
//////////////////////////////////////////////////////////////////

arete::arete(const point &a, const point &b, int na, int nb, int num)
:m_a(a), m_b(b), m_num_a(na), m_num_b(nb), m_num(num), m_inter(false)
{
    m_triangles.clear();
    m_taille=std::sqrt( produit_scalaire(b-a,b-a) );
}

point& arete::operator[](int i) 
{
    if (i==0)
        return m_a;
    if(i==1)
        return m_b;

    std::cout<<"Mauvaise numérotation [] arete" <<std::endl;
    exit(0);
}

point arete::operator[](int i) const
{
    if (i==0)
        return m_a;
    if(i==1)
        return m_b;
        
    std::cout<<"Mauvaise numérotation [] arete" <<std::endl;
    exit(0);
}

int& arete::operator()(int i)
{
    if(i<0 || i>=m_triangles.size() )
    {
        std::cout<<"Mauvaise numérotation () arete"<<std::endl;
    }
    return m_triangles[i];
}
int arete::operator()(int i) const
{
    if(i<0 || i>=m_triangles.size())
    {
        std::cout<<"Mauvaise numérotation () arete"<<std::endl;
    }
    return m_triangles[i];
}

bool arete::est_interieure() const
{
    return m_inter;
}

int arete::nb_triangles() const
{
    return m_triangles.size();
}

real arete::get_taille() const
{
    return m_taille;
}

void arete::ajoute_triangle(int i)
{
    m_triangles.push_back(i);
}

void arete::change_num(int i)
{
    m_num=i;
}

void arete::change_interieur(bool b)
{
    m_inter=b;
}

void arete::enleve_triangle(int i)
{
    for(int k=0; k<m_triangles.size(); k++)
    {
        if(m_triangles[k]==i)
        {
            m_triangles.erase(m_triangles.begin()+k);
            k--;
        }
    }
}

bool operator==(const arete& a1, const arete& a2)
{
    if(a1[0]==a2[0] && a1[1]==a2[1])
        return true;

    if(a1[0]==a2[1] && a1[1]==a2[0])
        return true;
    
    return false;
}

bool operator!=(const arete& a1, const arete& a2)
{
    return !(a1==a2);
}


