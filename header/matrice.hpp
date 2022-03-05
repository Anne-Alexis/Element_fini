#ifndef MATRICE_HPP
#define MATRICE_HPP

#include <vector>
#include <utility>
#include <iostream>
#include <cmath>

//CLASSE MATRICE m*n

template <typename T> class matrice
{
    protected:
        std::vector<T> m_valeurs;
        int m_n; //nombre de lignes
        int m_m; //nombre de colonnes

    public:
        //Constructeurs
        matrice(int n, int m);
        matrice(int n, int m, std::vector<T>);
        
        //Accesseurs
        std::pair<int,int> size() const;
        int nb_lignes() const;
        int nb_colonnes() const;
        std::vector<T> get_valeurs() const;

        //operateurs internes
        T& operator[](int);
        T operator[](int) const;
        T& operator()(int, int); //indexé à 1
        T operator()(int, int) const;

        matrice<T>& operator+=(const matrice<T>&);
        matrice<T>& operator-=(const matrice<T>&);
        matrice<T>& operator*=(const T&);//Multiplication par un scalaire
        matrice<T>& operator/=(const T&);//Division par un scalaire

        //Autres methodes

        void echange_lignes(int, int);
        void difference_lignes(int i, int j, T lb);// Li-lb*Lj
        T norme2();
};

template <typename T> matrice<T> identite(int);

/*  Operateurs externes:
    +,-, * par un scalaire à gauche et à droite, / par un scalaire à droite
    * produit matriciel
    >,>=,<,<=,==,!=
    affichage <<
*/

/////////Implémentation des constructeurs////////////

template <typename T> matrice<T>::matrice(int n, int m)
:m_n(n),m_m(m)
{
    if(m*n==0)
    {
        std::cout<<"Impossible de faire une matrice de taille nulle"<<std::endl;
        std::exit(0);
    }
    m_valeurs.resize(n*m);
}

template <typename T> matrice<T>::matrice(int n, int m, std::vector<T> valeurs)
:m_n(n),m_m(m),m_valeurs(valeurs)
{
   if(m*n != valeurs.size() || m*n==0)
   {
        std::cout<<"Taille de matrice incohérente"<<std::endl;
        std::exit(0);
   }
}

//////////Implementation des accesseurs//////////////////////

template <typename T> std::pair<int,int> matrice<T>::size() const
    {return std::make_pair(m_n,m_m);}

template <typename T> int matrice<T>::nb_lignes() const
    {return m_n;}

template <typename T> int matrice<T>::nb_colonnes() const
    {return m_m;}

template <typename T> std::vector<T> matrice<T>::get_valeurs() const
    {return m_valeurs;}

///////////////////Implémentations des operateurs///////////////

template <typename T> T& matrice<T>::operator[](int i)
{
    if(i<0 || i>=m_m*m_n)
    {
        std::cout<<"Mauvaise indexation operator[]"<<std::endl;
        exit(0);
    }
    return m_valeurs[i];
}

template <typename T> T matrice<T>::operator[](int i) const
{
    if(i<0 || i>=m_m*m_n)
    {
        std::cout<<"Mauvaise indexation operator[]"<<std::endl;
        exit(0);
    }
    return m_valeurs[i];
}

template <typename T> T& matrice<T>::operator()(int i, int j)
    {
        if(i<1 || i>m_n || j<1 || j>m_m)
        {
            std::cout<<"Matrice() mauvaise numérotation"<<std::endl;
            exit(0);
        }
        return (*this)[(i-1)*m_m + j-1];
    }

template<typename T> T matrice<T>::operator()(int i, int j) const
    {
        if(i<1 || i>m_n || j<1 || j>m_m)
        {
            std::cout<<"Matrice () mauvaise numérotation"<<std::endl;
            exit(0);
        }
        return  (*this)[(i-1)*m_m + j-1];
    }

template <typename T> matrice<T>& matrice<T>::operator+=(const matrice<T>& B)
{
    if(B.size() != this->size() )
    {
        std::cout<<"Addition, matrices de tailles différentes"<<std::endl;
        std::exit(0);
    }
    for(int i=0; i<m_valeurs.size() ;i++)
        {m_valeurs[i]+=B[i];}
    return *this;
}

template <typename T> matrice<T>& matrice<T>::operator-=(const matrice<T>& B)
{
    if(B.size() != this->size() )
    {
       std::cout<<"Addition, matrices de tailles différentes"<<std::endl;
       std::exit(0);
    }
    for(int i=0; i<m_valeurs.size() ;i++)
        {m_valeurs[i]-=B[i];}
    return *this;
}

template <typename T> matrice<T>& matrice<T>::operator*=(const T& sc)
{
    for(int i=0; i<m_valeurs.size() ;i++)
        {m_valeurs[i]*=sc;}
    return *this;
}

template <typename T> matrice<T>& matrice<T>::operator/=(const T& sc)
{
    for(int i=0; i<m_valeurs.size() ;i++)
        {m_valeurs[i]*=(1/sc);}
    return *this;
}

template <typename T> matrice<T> operator+(const matrice<T>& A,const matrice<T>& B)
{
    matrice<T> res=A; 
    return res+=B;
}

template <typename T> matrice<T> operator-(const matrice<T>& A,const matrice<T>& B)
{
    matrice<T> res=A; 
    return res-=B;
}

template <typename T> matrice<T> operator*(const matrice<T>& A,const T& sc)
{
    matrice<T> res=A;
    return res*=sc;
}

template <typename T> matrice<T> operator*(const T& sc,const matrice<T>& A)
    {return A*sc; }

template <typename T> matrice<T> operator/(const matrice<T>& A, const T& sc)
{
    matrice<T> res=A;
    return res/=sc;
}

template <typename T> std::ostream& operator<<(std::ostream& out, const matrice<T>& A)
{
    for(int i=1; i<= A.nb_lignes(); i++)
    {
        for(int j=1; j<= A.nb_colonnes() ; j++)
        {
            if( A(i,j)>=0.0 )
                {out<<" ";}
            out<<A(i,j)<<" ";
        }
        out<<std::endl;
    }
    return out;
}

//Opérateurs de comparaison

template<typename T> bool operator==(const matrice<T> &A, const matrice<T> &B)
{
    if(A.size!=B.size())
        {return false;}
    for(int i=0; i<A.nb_lignes(); i++)
    {
        for(int j=0; j<B.nb_colonnes(); j++)
        {
            if( A(i,j)!=B(i,j) )
                {return false;}
        }
    }
    return true;
}

template<typename T> bool operator!=(const matrice<T> &A, const matrice<T> &B)
    {return !(A==B);}

template<typename T> bool operator>=(const matrice<T> &A, const matrice<T> &B)
{
    if(A.size!=B.size())
        {return false;}
    for(int i=0; i<A.nb_lignes(); i++)
    {
        for(int j=0; j<B.nb_colonnes(); j++)
        {
            if( A(i,j) < B(i,j) )
            {return false;}
        }
    }
    return true;
}

template<typename T> bool operator<=(const matrice<T> &A, const matrice<T> &B)
{
    return B>=A;
}

template<typename T> bool operator<(const matrice<T> &A, const matrice<T> &B)
{
    if(A.size!=B.size())
        {return false;}
    for(int i=0; i<A.nb_lignes(); i++)
    {
        for(int j=0; j<B.nb_colonnes(); j++)
        {
            if( A(i,j) >= B(i,j) )
            {return false;}
        }
    }
    return true;
}

template<typename T> bool operator>(const matrice<T> &A, const matrice<T> &B)
{
    return B<A;
}

//Produit matriciel

template<typename T> matrice<T> operator*(const matrice<T> &A, const matrice<T>& B)
{
    if(A.nb_colonnes() != B.nb_lignes() )
    {   
        std::cout<<"Pb dimension"<<std::endl; 
        throw ("Pb dimension");
    }
    matrice<T> res(A.nb_lignes() ,B.nb_colonnes() );
    for(int i=1; i<=res.nb_lignes(); i++)
    {
        for(int j=1; j<=res.nb_colonnes(); j++)
        {
            res(i,j)=(T) 0;
            for(int k=1; k<=B.nb_lignes() ; k++)
                {res(i,j)+=A(i,k)*B(k,j);}
        }
    }
    return res;
}

//Implémentation des méthodes

//Créer la matrice identité de taille nxn
template <typename T> matrice<T> identite(int n)
{
    if(n<=0)
    {
        std::cout<< "Matrice identité taille négative ou nulle"<<std::endl;
        exit(0);
    }
    matrice<T> res(n,n);
    for(int i=1; i<=n ;i++)
    {
        for(int j=1; j<=n; j++)
        {
            i==j ? res(i,j) = (T) 1 : res(i,j) = (T) 0 ;
        }
    }
    return res;
}


template<typename T> void matrice<T>::echange_lignes(int i, int j)
{
    if(i<=0 || j<=0 || i>m_n || j>m_n)
    {
        std::cout<<"Numero ligne incorrecte"<<std::endl;
        std::exit(0);
    }
    T tp;
    for(int k=1; k<=m_m; k++)
    {
        tp=(*this)(i,k);
        (*this)(i,k)=(*this)(j,k);
        (*this)(j,k)=tp;
    }
}

template<typename T> void matrice<T>::difference_lignes(int i, int j, T multiplicateur)
{
    if(i<=0 || j<=0 || i>m_n || j>m_n)
    {
        std::cout<<"Numero ligne incorrecte"<<std::endl;
        std::exit(0);
    }
    for(int k=1; k<=m_m; k++)
    {
        (*this)(i,k) -= multiplicateur*(*this)(j,k);
    }
}

template<typename T> T matrice<T>::norme2()
{
    T res(0);
    for(int i=0; i<m_valeurs.size(); i++)
    {
        res+=m_valeurs[i]*m_valeurs[i];
    }
    return std::sqrt(res)/(m_n*m_m);
}

//Inverse matrice carrée, méthode directe pivot de Gauss
template<typename T> matrice<T> inverse(matrice<T> A) 
{
    //On copie volontairement la matrice d'entrée pour pouvoir la modifier sans problème
    if(A.nb_colonnes()!=A.nb_lignes() )
    {
        std::cout<<"Impossible de trouver l'inverse d'une matrice non carrée"<<std::endl;
        std::exit(0);
    }
    int n = A.nb_colonnes();
    matrice<T> res(identite<T>(n));    

    for(int j=1; j<=n; j++)
    {
        //On trouve le premier terme non nul de la colonne j
        int i=j;
        while( A(i,j)==0 && i<n)
            {i++;}
        if( A(i,j)==0)
        {
            std::cout<<"Matrice non inversible"<<std::endl;
            std::exit(0);    
        }
        //On place la ligne contenant ce terme en numéro j
        A.difference_lignes(j,i,(T) -1);
        res.difference_lignes(j,i,(T) -1);
        //On réduit à 1 le terme A(j,j)
        for(int k=j+1; k<=n ;k++)
            {A(j,k)/=A(j,j);}
        for(int k=1;k<=n;k++)
            {res(j,k)/=A(j,j);}
        A(j,j)=(T) 1;
        //On élimine la suite de la colonne
        for(int k=1; k<=n; k++)
        {
            if(k!=j)
            {   
                T multplicateur=A(k,j);
                A.difference_lignes(k,j, multplicateur);
                res.difference_lignes(k,j, multplicateur );
            }
        }
    }

    return res;
}

template<typename T> matrice<T> transpose(const matrice<T> &A)
{
    matrice<T> res(A.nb_colonnes,A.nb_lignes);
    for(int i=1; i<=res.nb_lignes(); i++)
    {
        for(int j=1; j<=res.nb_colonnes(); j++)
            {res(j,i)=A(i,j);}
    }
    return res;
}

#endif 