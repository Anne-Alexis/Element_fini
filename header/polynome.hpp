#ifndef POLYNOME_HPP
#define POLYNOME_HPP

#include <vector>
#include <iostream>
#include <cmath>

#include "./arithmetique.hpp"


//Classe templatée pour implémenter les polynomes

template <typename T> class polynome
//Polynomes d'un T-ev à valeurs dans U
{
    protected:
        int m_degre; //degré du polynome //-1 si poly nul
        std::vector<T> m_coefficients; //coeff organisé de 0 à m_degre 

    public:
        //constructeurs
        polynome(int degre);
        polynome(std::vector<T> coefficients);

        //Accesseurs
        int get_degre() const;
        std::vector<T> get_coeffs() const;

        //Surcharges d'opérateurs
        T operator[](int) const;
        T& operator[](int);

        polynome<T>& operator+=(const polynome<T>&);
        polynome<T>& operator-=(const polynome<T>&);
        polynome<T>& operator*=(const T&);              //Multiplication par un scalaire
        polynome<T>& operator*=(const polynome<T>&);    //Multiplication par un polynome
        polynome<T>& operator/=(const T&);              //Division par un scalaire


        //Modificateurs simples//

        void ajoute_coeff(const T&);
        void decalage(int m); // décale de m cran les coeff( multiplie par x^m)
        void ajuste_degre(); //Baisse le degré si termes nuls

        template <typename P, typename U> friend U eval(const polynome<P> &, const U&);
};


//Opérateurs externe//
/*
    ==, != , << 
    * par un scalaire gauche, droite
    / par un scalaire à droite
    +, -
    * par un polynome
*/
//////////////////////

///Autres fonctions//

template <typename T> polynome<T> berlekamp_massey(std::vector<T> S); //Algorithme berlecamp pour trouver le polynome annulateur
template<typename T> std::pair< polynome<T>, polynome<T> > div_euclide(polynome<T> A, const polynome<T> &B);



////////////constructeurs///////////////////
template <typename T> polynome<T>::polynome(int degre)
:m_degre(degre)
{
    m_coefficients.clear();
    if( degre < -1 )
    {
        std::cout<<"polynome::polynome mauvais degré"<<std::endl;
        exit(0);
    }
    
    m_coefficients.push_back(0);

    for(int i=0; i<degre; i++)
    {
        m_coefficients.push_back(0);
    }
    
}

template <typename T> polynome<T>::polynome(std::vector<T> coefficients)
:m_coefficients( coefficients ), m_degre( coefficients.size() - 1 )
{
    if(coefficients.size()==1 && coefficients[0]==0)
        m_degre=-1;
}
        
//////////////Accesseurs/////////////////////
template <typename T> int polynome<T>::get_degre() const
{
    return m_degre;
}

template <typename T> std::vector<T> polynome<T>::get_coeffs() const
{
    return m_coefficients;
}

///////////Surcharges d'opérateurs internes/////
template <typename T> T polynome<T>::operator[](int i) const
{
    if( i<0 || i> m_degre)
    {
        std::cout<<"polynome [] const pb indice"<<std::endl;
        exit(0);
    }
    return m_coefficients[i];
}

template <typename T> T& polynome<T>::operator[](int i)
{
    if( i<0 || i>m_degre)
    {
        std::cout<<"polynome [] const pb indice"<<std::endl;
        exit(0);
    }
    return m_coefficients[i];    
}

template <typename T> polynome<T>& polynome<T>::operator+=(const polynome<T>& P)
{
    int m1( P.get_degre() );
    if(m1==-1)
    {
        return *this;
    }
        

    int max(0); int min(0);
    m1 > m_degre ? max = m1 : max = m_degre ;
    m1 > m_degre ? min = m_degre : min = m1 ;

    int i=0;
    for(i=0; i<=min; i++)
    {
        m_coefficients[i]+=P[i];
    }
    if(m1==max)
    {
        for(i=min+1; i<=max; i++)
        {
            m_coefficients.push_back(P[i]);
        }
        m_degre=m1;
    }
    this->ajuste_degre();
    return *this;
}

template <typename T> polynome<T>& polynome<T>::operator-=(const polynome<T>& P)
{
    int m1(P.get_degre());
    if(m1==-1)
        return *this;

    int max(0); int min(0);
    m1 > m_degre ? max = m1 : max = m_degre ;
    m1 > m_degre ? min = m_degre :  min = m1;

    int i=0;
    for(i; i<=min; i++)
    {
        m_coefficients[i]-=P[i];
    }
    if(m1==max)
    {
        for(i=min+1; i<=max; i++)
        {
            m_coefficients.push_back(-P[i]);
        }
        m_degre=m1;
    }
    this->ajuste_degre();
    return *this;
}   

template <typename T> polynome<T>& polynome<T>::operator*=(const polynome<T>& P)    //Multiplication par un polynome
{
    int m1=m_degre;
    int m2=P.get_degre();
    m_degre=m1+m2;

    if(m1==-1 || m2==-1)
    {
        m_coefficients.clear();
        m_coefficients.push_back(0);
        m_degre=-1;
        return *this;
    }

    std::vector<T> produit(m1+m2+1);

    for(int i=0; i<=m1; i++)
    {
        for(int j=0; j<=m2; j++)
        {
            produit[i+j]+=(*this)[i]*P[j];
        }
    }
    m_coefficients=produit;
    return *this;
}

template <typename T> polynome<T>& polynome<T>::operator*=(const T& a)              //Multiplication par un scalaire
{
    if(m_degre==-1)
        return *this;
    if(a==0)
    {
        m_coefficients.clear();
        m_coefficients.push_back(0);
        m_degre=-1;
        return *this;
    }

    for(int i=0; i<m_coefficients.size(); i++)
        m_coefficients[i]*= a;
    return *this;
}

template <typename T> polynome<T>& polynome<T>::operator/=(const T& a)              //Division par un scalaire
{
    if(m_degre==-1)
        return *this;
    if( a==0 )
    {
        std::cout<< "polynome /= division par 0 "<<std::endl;
    }
    for(int i=0; i<m_coefficients.size(); i++)
        m_coefficients[i]/= a;
    return *this;
}

////////////Modificateurs simples///////////////

template <typename T> void polynome<T>::ajoute_coeff(const T& a)
{
    m_degre++;
    m_coefficients.push_back(a);
}

template <typename T> void  polynome<T>::decalage(int m) // décale de m cran les coeff( multiplie par x^m)
{
    if(m>0)
    {
        m_degre+=m;
        m_coefficients.insert(m_coefficients.begin(), m, (T) 0);
    }
}

template <typename T> void polynome<T>::ajuste_degre()
{
    while( !m_coefficients.empty() && m_coefficients.back()< 0.00000000001 && m_coefficients.back() > -0.00000000001)
    {
        m_degre--;
        m_coefficients.pop_back();
    }
    if(m_coefficients.empty() )
    {
        m_coefficients.push_back(0);
        m_degre=-1;
    }
}

/////////Surcharges d'opérateurs externes////////
    

template<typename T> bool operator==(const polynome<T> &P,const polynome<T> &Q)
{
    if( P.get_degre()!=Q.get_degre() )
        return false;
    int n=P.get_degre();
    for(int i=0; i<=n; i++)
    {
        if(P[i]!=Q[i])
            return false;
    }
    return true;
}

template<typename T> bool operator!=(const polynome<T> &P,const polynome<T> &Q)
{
    return !(P==Q);
}

template<typename T> polynome<T> operator*(const polynome<T> &P,const polynome<T> &Q)
{
    polynome<T> res(P);
    return res*=Q;
}

template<typename T> polynome<T> operator*(const polynome<T> &P,const T &a)
{
    polynome<T> res(P);
    return res*=a;
}

template<typename T> polynome<T> operator*(const T &a, const polynome<T> &P)
{
    polynome<T> res(P);
    return res*=a;
}

template<typename T> polynome<T> operator+(const polynome<T> &P,const polynome<T> &Q)
{
    if(P.get_degre()==-1)
    {return Q;}

    polynome<T> res(P);
    return res+=Q;
}

template<typename T> polynome<T> operator-(const polynome<T> &P,const polynome<T> &Q)
{
    if(P.get_degre()==-1)
    {
        return Q*((T) -1);
    }

    polynome<T> res(P);
    return res-=Q;
}


template<typename T>polynome<T> operator/(const polynome<T> &P,const T &a)
{
    polynome<T> res(P);
    return res/=a;    
}

template<typename T> std::ostream& operator<<(std::ostream& out , const polynome<T> &P)
{
    if(P.get_degre()==-1)
    {
        out<<"Polynôme nul"<<std::endl;
        return out;
    }

    for(int i=0; i<=P.get_degre(); i++)
        out<<"rang "<<i<<" : "<< P[i] << std::endl;
    return out;
}

//Quelques fonctions

template<typename T> std::pair< polynome<T>, polynome<T> > div_euclide(polynome<T> A, const polynome<T> &B)
{
    int na=A.get_degre();
    int nb=B.get_degre();

    polynome<T> Q(na-nb);

    if(na < nb )
    {
        Q.ajuste_degre();
        return std::make_pair( Q, B);
    }

    int cpt(0);
    while( na>=nb && cpt<4)
    {
        cpt++;
        polynome<T> D(B);
        D.decalage(na-nb);
        T c=A[na]/B[nb];
        D*=c;
        A -= D;
        A.ajuste_degre();
        Q[na-nb]= c;

        na=A.get_degre();
    }
    Q.ajuste_degre();
    return std::make_pair(Q,A);
}

template <typename T> polynome<T> berlekamp_massey(std::vector<T> s)
{
    int n=s.size();
    if(s.size()%2==1)
    {
        std::cout<<"berlekamp_massey pb dim s impair"<<std::endl;
    }
    polynome<T> S(s);
    polynome<T> C( pade_n(S) );
    return C;
}


#endif