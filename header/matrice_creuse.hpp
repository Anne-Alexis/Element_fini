#ifndef matrice_creuse_HPP
#define matrice_creuse_HPP

#include <vector>
#include <utility>
#include <iostream>
#include <cmath>

//CLASSE matrice_creuse m*n

template <typename T> class matrice_creuse
{
    protected:
        int m_n; //nombre de lignes
        int m_m; //nombre de colonnes
        std::vector<T> m_valeurs;
        std::vector<int> m_indices_i; //liste triées des numéros de lignes (commencant à 1)
        std::vector<int> m_indices_j; //liste des numéros de colonnes triées intérieurement (commencant à 1)

        void ajoute_coeff_fin(T val, int i, int j); //rajoute en fin de liste le nouveau coeff

    public:
        //Constructeurs
        matrice_creuse(int n, int m);

        std::pair<int,int> size() const;
        int nb_lignes() const;
        int nb_colonnes() const;
        std::vector<T> get_valeurs() const;

        //Accesseurs
        int nb_coefficients() const;
        std::vector<int> get_indices_i() const;
        std::vector<int> get_indices_j() const;
        std::pair<int,int> get_indices(int) const; //renvoie la pair des coeffs
        int get_valeur(int) const; //renvoi m_valeurs[i]
        int num_coeff(int i, int j) const; //renvoie le numéro dans les listes de coefficient (i,j)
                                           //renvoie -1 s'il n'est pas référencé

        ///operateurs internes
        T& operator[](int);
        T operator[](int) const;
        T& operator()(int, int); //indexé à 1
        T operator()(int, int) const;

        //Modificateurs simples
        void def_coeff(T,int,int);

        matrice_creuse<T>& operator+=(const matrice_creuse<T>&);
        matrice_creuse<T>& operator-=(const matrice_creuse<T>&);
        matrice_creuse<T>& operator*=(const T&);//Multiplication par un scalaire
        matrice_creuse<T>& operator/=(const T&);//Division par un scalaire

        ///Autres methodes
        //void echange_lignes(int, int);
        //void difference_lignes(int i, int j, T lb);// Li-lb*Lj

        //Friends
        template<typename U> friend matrice_creuse<U> identite_cr(int n);
        template<typename U> friend matrice_creuse<U> transpose(const matrice_creuse<U> &);
        template<typename U> friend matrice_creuse<U> operator*(const matrice_creuse<U> &, const matrice_creuse<U> &);
};

void tri_fusion(std::vector<int> &, std::vector<int> &);



//Operateurs externes
/*
    +,-, * par un scalaire à gauche et à droite, / par un scalaire à droite
    * produit matriciel
    >,>=,<,<=,==,!=
    affichage <<

*/

//Implémentation des constructeurs
template <typename T> matrice_creuse<T>::matrice_creuse(int n, int m)
:m_n(n),m_m(m)
{
    if(n<=0 || m<=0 )
    {
        std::cout<<"matrice_creuse::matrice_creuse dimension <=0"<<std::endl;
    }
    m_valeurs.clear();
    m_indices_i.clear();
    m_indices_j.clear();
}


//////////////////Méthodes privées////////////////

template <typename T> void matrice_creuse<T>::ajoute_coeff_fin(T val, int i, int j)
{
    m_valeurs.push_back(val);
    m_indices_i.push_back(i);
    m_indices_j.push_back(j);
}


///////////////////Accesseurs//////////////////////

template <typename T> std::pair<int,int> matrice_creuse<T>::size() const
{return std::make_pair(m_n,m_m);}

template <typename T> int matrice_creuse<T>::nb_lignes() const
{return m_n;}

template <typename T> int matrice_creuse<T>::nb_colonnes() const
{return m_m;}

template <typename T> std::vector<T> matrice_creuse<T>::get_valeurs() const
{return m_valeurs;}



template <typename T> int matrice_creuse<T>::num_coeff(int i, int j) const
{
    //Les numéros de lignes forment une liste triée => recherche par dichotomie
    int N=m_indices_i.size();

    if(i<=0 || j<=0 || i>m_n || j>m_m)
    {
        std::cout<<"Num_coeff Numero ligne incorrecte"<<std::endl;
        std::exit(0);
    }

    int n=(N-1)/2;
    int ng= 0;
    int nd= N-1;

    while( m_indices_i[n]!=i && nd-ng>1)
    {
        if(m_indices_i[n]>i)
        {
            nd=n;
            n=(nd+ng)/2;
        }
        else
        {
            ng=n;
            n=(nd+ng)/2;
        }
    }

    if(nd-ng<=1)
    {
        if(m_indices_i[ng]==i)
        {n=ng;}
        else if(m_indices_i[nd]==i)
        {n=nd;}
    }

    if(m_indices_i[n]==i)
    {
        //Maintenant qu'on a identifié un N de numéro de ligne i, il faut tous les trouver, et trouver l'intervalle [n_min, n_max] où ils sont
        int n_min=n;
        int n_max=n;

        while( m_indices_i[n_min]==i )
        {n_min--;}
        n_min++;
        while( m_indices_i[n_max]==i )
        {n_max++;}
        n_max--;

        for(n=n_min; n<=n_max; n++)
        {
            if(m_indices_j[n]==j)
            {return n;}        
        }
    }
    return -1;
}

template<typename T> int matrice_creuse<T>::nb_coefficients() const
{return m_indices_i.size();}

template<typename T> std::vector<int> matrice_creuse<T>::get_indices_i() const
{return m_indices_i;}

template<typename T> int matrice_creuse<T>::get_valeur(int i) const
{return m_valeurs[i];}


template<typename T> std::vector<int> matrice_creuse<T>::get_indices_j() const
{return m_indices_j;}

template<typename T> std::pair<int,int> matrice_creuse<T>::get_indices(int i) const
{
    if( i<0 || i>m_valeurs.size())
    {
        std::cout<<"Indice impossible get_indice"<<std::endl;
        std::exit(0);
    }
    return std::make_pair( m_indices_i[i], m_indices_j[i] );
}

////////////////Modificateurs simples//////////////////

template <typename T> void matrice_creuse<T>::def_coeff(T val, int i, int j)
{
    
    if(i<=0 || j<=0 || i>m_n || j>m_m)
    {
        std::cout<<"matrice_creuse::def_coeff numero de coeff incorrect"<<std::endl;
        std::exit(0);
    }

    if(m_indices_i.size()==0)
    {
        m_valeurs.push_back(val);
        m_indices_i.push_back(i);
        m_indices_j.push_back(j);
    }
    else
    {
        int N=m_indices_i.size();
        int n=(N-1)/2;
        int ng= 0;
        int nd= N-1;

        while( m_indices_i[n]!=i && nd-ng>1)
        {
            if(m_indices_i[n]>i)
            {
                nd=n;
                n=(nd+ng)/2;
            }else{
                ng=n;
                n=(nd+ng)/2;
            }
        }
        if(nd-ng<=1)
        {
            if(m_indices_i[ng]==i)
            {n=ng;}
            else if(m_indices_i[nd]==i)
            {n=nd;}
        }

        if(m_indices_i[n]<i)
        {
            m_valeurs.emplace(m_valeurs.begin()+ n + 1 ,val);
            m_indices_i.emplace(m_indices_i.begin()+ n + 1 ,i);
            m_indices_j.emplace(m_indices_j.begin()+ n + 1 ,j);
        }
        else if(m_indices_i[n]>i)
        {
            m_valeurs.emplace(m_valeurs.begin()+ n ,val);
            m_indices_i.emplace(m_indices_i.begin()+ n ,i);
            m_indices_j.emplace(m_indices_j.begin()+ n ,j);
        }
        else
        {
            //Maintenant qu'on a identifié un N de numéro de ligne i, il faut tous les trouver, et trouver l'intervalle [n_min, n_max] où ils sont
            int n_min=n;
            int n_max=n;

            while( m_indices_i[n_min]==i )
            {n_min--;}
            n_min++;
            while( m_indices_i[n_max]==i )
            {n_max++;}
            n_max--;

            if(m_indices_j[n_min]>j)
            {
                m_valeurs.emplace(m_valeurs.begin()+ n_min   ,val);
                m_indices_i.emplace(m_indices_i.begin()+ n_min ,i);
                m_indices_j.emplace(m_indices_j.begin()+ n_min ,j);
            }
            else if(m_indices_j[n_max]<j)
            {
                m_valeurs.emplace(m_valeurs.begin()+ n_max +1    ,val);
                m_indices_i.emplace(m_indices_i.begin()+ n_max +1  ,i);
                m_indices_j.emplace(m_indices_j.begin()+ n_max  +1 ,j);
            }
            else
            {
                for(n=n_min; n<=n_max; n++)
                {
                    if(m_indices_j[n]==j)
                    {
                        m_valeurs[n]=val;
                    } 
                    else if(n > n_min && m_indices_j[n-1]<j && m_indices_j[n]>j )
                    {
                        m_valeurs.emplace(m_valeurs.begin()+ n  ,val);
                        m_indices_i.emplace(m_indices_i.begin()+ n  ,i);
                        m_indices_j.emplace(m_indices_j.begin()+ n  ,j);
                    }
                }
            }
        }
    }
}

//////////////Surcharges opérateurs///////////////////////

template <typename T> T& matrice_creuse<T>::operator[](int i)
{
    int l=i/m_m;
    int c=i%m_m;
    if(i<0 || i>=m_m*m_n)
    {
        std::cout<<"Mauvaise indexation operator[]"<<std::endl;
        exit(0);
    }
    int n=this->num_coeff(l+1,c+1);
    if(n==-1)
        {return (T) 0;}
    return m_valeurs[n];
}

template <typename T> T matrice_creuse<T>::operator[](int i) const
{    
    int l=i/m_m;
    int c=i%m_m;
    if(i<0 || i>=m_m*m_n)
    {
        std::cout<<"Mauvaise indexation operator[]"<<std::endl;
        exit(0);
    }
    int n=this->num_coeff(l+1,c+1);
    if(n==-1)
        {return (T) 0;}
    return m_valeurs[n];
}

template <typename T> T&  matrice_creuse<T>::operator()(int i, int j)
{return (*this)[(i-1)*m_m + j-1];}

template <typename T> T matrice_creuse<T>::operator()(int i, int j) const
{return (*this)[(i-1)*m_m + j-1];}

template <typename T> matrice_creuse<T>& matrice_creuse<T>::operator+=(const matrice_creuse<T>& B)
{
    if(B.size() != this->size() )
    {
        std::cout<<"Addition, matrice_creuses de tailles différentes"<<std::endl;
        std::exit(0);
    }
    //Parcours en parallèle des listes de A et B, si le coeff est seulement dans un des deux points on rajoute le point
    //Si la somme fait 0 on supprime le point
    matrice_creuse<T> A=(*this);
    m_valeurs.clear();
    m_indices_i.clear();
    m_indices_j.clear();
    int cpt1(0); 
    int cpt2(0);
    int ia, ib, ja, jb;

    while(cpt1<A.nb_coefficients() && cpt2<B.nb_coefficients() )
    {
        ia=A.get_indices(cpt1).first;
        ib=B.get_indices(cpt2).first;
        ja=A.get_indices(cpt1).second;
        jb=B.get_indices(cpt2).second;
        if(ia==ib && ja==jb)
        {
            m_valeurs.push_back(A.get_valeur(cpt1) + B.get_valeur(cpt2));
            m_indices_i.push_back(ia);
            m_indices_j.push_back(ja);
            cpt1++;
            cpt2++;
        }
        else if( ia<ib || (ia==ib && ja<jb) )
        {
            m_valeurs.push_back( A.get_valeur(cpt1) );
            m_indices_i.push_back(ia);
            m_indices_j.push_back(ja);
            cpt1++;
        }
        else
        {
            m_valeurs.push_back( B.get_valeur(cpt1) );
            m_indices_i.push_back(ib);
            m_indices_j.push_back(jb);
            cpt2++;
        }
    }

    if(cpt1< A.nb_coefficients() )
    {
        for(int k=cpt1; k<A.nb_coefficients(); k++)
        {
            m_valeurs.push_back(A.get_valeur(k));
            m_indices_i.push_back(A.get_indices(k).first);
            m_indices_j.push_back(A.get_indices(k).second);
        }
    }
    else if(cpt2< B.nb_coefficients())
    {
        for(int k=cpt2; k<B.nb_coefficients(); k++)
        {
            m_valeurs.push_back(B.get_valeur(k));
            m_indices_i.push_back(B.get_indices(k).first);
            m_indices_j.push_back(B.get_indices(k).second);
        }
    }
    return *this;
}

template <typename T> matrice_creuse<T>& matrice_creuse<T>::operator-=(const matrice_creuse<T>& B)
{
    (*this)+= -1*B;
    return *this;
}

template <typename T> matrice_creuse<T>& matrice_creuse<T>::operator*=(const T& sc)
{
    for(int i=0; i<m_valeurs.size() ;i++)
        {m_valeurs[i]*=sc;}
    return *this;
}

template <typename T> matrice_creuse<T>& matrice_creuse<T>::operator/=(const T& sc)
{
    for(int i=0; i<m_valeurs.size() ;i++)
        {m_valeurs[i]*=(1/sc);}
    return *this;
}

template <typename T> matrice_creuse<T> operator+(const matrice_creuse<T>& A,const matrice_creuse<T>& B)
{
    matrice_creuse<T> res=A; 
    return res+=B;
}

template <typename T> matrice_creuse<T> operator-(const matrice_creuse<T>& A,const matrice_creuse<T>& B)
{
    matrice_creuse<T> res=A; 
    return res-=B;
}

template <typename T> matrice_creuse<T> operator*(const matrice_creuse<T>& A,const T& sc)
{
    matrice_creuse<T> res=A;
    return res*=sc;
}

template <typename T> matrice_creuse<T> operator*(const T& sc,const matrice_creuse<T>& A)
    {return A*sc; }

template <typename T> matrice_creuse<T> operator/(const matrice_creuse<T>& A, const T& sc)
{
    matrice_creuse<T> res=A;
    return res/=sc;
}

template <typename T> std::ostream& operator<<(std::ostream& out, const matrice_creuse<T>& A)
{
    for(int i=0; i< A.nb_coefficients(); i++)
    {
        std::pair<int,int> c=A.get_indices(i);
        out<< "(" << c.first << "," << c.second << ") : " << A.get_valeur(i) <<  std::endl;
    }
    return out;
}

//Opérateurs de comparaison

template<typename T> bool operator==(const matrice_creuse<T> &A, const matrice_creuse<T> &B)
{
    if(A.size!=B.size())
    {return false;}

    for(int i=0; i<A.nb_coefficients; i++)
    {
        if(A.get_valeur(i)!=B.get_valeur(i) || A.get_indices(i)!=B.get_indices(i) )
        {
            return false;
        }
    }
    return true;
}

template<typename T> bool operator!=(const matrice_creuse<T> &A, const matrice_creuse<T> &B)
    {return !(A==B);}

template<typename T> bool operator>=(const matrice_creuse<T> &A, const matrice_creuse<T> &B)
{
    if(A.size!=B.size())
        {return false;}
    for(int i=0; i<A.nb_coefficients(); i++)
    {
        if(A.get_indices(i)!=B.get_indices(i) || A.get_valeur(i)<B.get_valeur(i) )
            {return false;}
    }
    return true;
}

template<typename T> bool operator<=(const matrice_creuse<T> &A, const matrice_creuse<T> &B)
{
    return B>=A;
}

template<typename T> bool operator<(const matrice_creuse<T> &A, const matrice_creuse<T> &B)
{
    if(A.size!=B.size())
        {return false;}
    for(int i=0; i<A.nb_coefficients(); i++)
    {
        if(A.get_indices(i)!=B.get_indices(i) || A.get_valeur(i)>=B.get_valeur(i) )
            {return false;}
    }
    return true;
}

template<typename T> bool operator>(const matrice_creuse<T> &A, const matrice_creuse<T> &B)
{
    return B<A;
}

//Produit matriciel
template<typename T> matrice_creuse<T> operator*(const matrice_creuse<T>& A, const matrice_creuse<T>& B)
{
    if(A.nb_colonnes() != B.nb_lignes() )
    {   
        std::cout<<"Matrice_creuse::produit matriciel pb dimension"<<std::endl; 
        exit(0);
    }
    matrice_creuse<T> res(A.nb_lignes() ,B.nb_colonnes() );
    matrice_creuse<T> Bt=transpose(B);
    //
    int na=0;
    int nb=0;
    int NA=A.nb_coefficients();
    int NB=Bt.nb_coefficients();
    for(int i=1; i<=res.nb_lignes(); i++)
    {
        int indice_debut_ligne=na;
        nb=0;
        if(na<NA && A.m_indices_i[na]==i)
        {
            for(int j=1; j<=res.nb_colonnes(); j++)
            {
                na= indice_debut_ligne;
                while(na<NA && nb<NB && A.m_indices_i[na]==i && Bt.m_indices_i[nb]==j)
                {
                    if(A.m_indices_j[na]==Bt.m_indices_j[nb])
                    {
                        if( !res.m_indices_i.empty() &&res.m_indices_i.back()==i && res.m_indices_j.back()==j)
                        {
                            res.m_valeurs.back()+=A.m_valeurs[na]*Bt.m_valeurs[nb];
                        }
                        else
                        {
                            res.m_indices_i.push_back(i);
                            res.m_indices_j.push_back(j);
                            res.m_valeurs.push_back( A.m_valeurs[na]*Bt.m_valeurs[nb] );
                        }
                    }
                    na++;
                    nb++;
                }
                while(nb<NB && Bt.m_indices_i[nb]==j)
                    nb++;
            }
        }      
    }
    return res;
}

//Implémentation des méthodes

//Créer la matrice_creuse identité de taille nxn
template<typename T> matrice_creuse<T> identite_cr(int n)
{
    if(n<=0)
    {
        std::cout<<"Matrice_creuse identité de taille <= 0"<<std::endl;
        std::exit(0);
    }
    matrice_creuse<T> idn(n,n);
    for(int i=1; i<=n; i++)
    {
        idn.ajoute_coeff_fin( (T) 1, i, i);
    }
    return idn;
}

template<typename T> matrice_creuse<T> transpose(const matrice_creuse<T> &A)
{
    matrice_creuse<T> res(A.nb_colonnes(), A.nb_lignes() );

    std::vector<int> indi=A.m_indices_i;
    std::vector<int> indj=A.m_indices_j;

    tri_fusion(indj, indi);
    
    for(int i=0; i<indi.size(); i++)
    {
        res.ajoute_coeff_fin( A( indi[i], indj[i] ), indj[i] , indi[i]);
    }

    return res;
}

void tri_fusion(std::vector<int> &v1, std::vector<int> &v2)
{
    if(v1.size()!= v2.size())
    {
        std::cout<<"tri_fusion v1.size!=v2.size()"<<std::endl;
        exit(0);
    }
    int N=v1.size();
    if(N > 1)
    {
        std::vector<int> nv1g = {v1.begin(), v1.begin() + N/2} ;
        std::vector<int> nv2g = {v2.begin(), v2.begin() + N/2} ;
        std::vector<int> nv1d = {v1.begin() + N/2, v1.end()} ;
        std::vector<int> nv2d = {v2.begin() + N/2, v2.end()} ;

        tri_fusion(nv1g,nv2g);
        tri_fusion(nv1d,nv2d);

        int i=0; int j=0;
        while(i<nv1g.size() && j<nv1d.size() )
        {
            if(nv1g[i] <= nv1d[j] )
            {
                v1[i+j]=nv1g[i];
                v2[i+j]=nv2g[i];
                i++;
            }
            else
            {
                v1[i+j]=nv1d[j];
                v2[i+j]=nv2d[j];
                j++;
            }
        }

        if(i<nv1g.size())
        {
            for(i; i< nv1g.size(); i++)
            {
                v1[i+j]=nv1g[i];
                v2[i+j]=nv2g[i];
            }
        }
        else if (j<nv1d.size())
        {
            for(j; j<nv1d.size(); j++)
            {
                v1[i+j]=nv1d[j];
                v2[i+j]=nv2d[j];
            }
        }
    }
}

#endif 