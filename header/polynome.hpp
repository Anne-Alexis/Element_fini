#ifndef POLYNOME_HPP
#define POLYNOME_HPP

#include <vector>
#include <iostream>

//Classe templatée pour implémenter les polynomes

template <typename T> class polynome
//Polynomes d'un T-ev à valeurs dans U
{
    protected:
        int m_degre; //degré du polynome
        std::vector<T> m_coefficients; //coeff organisé de 0 à m_degre 

    public:
        //constructeurs
        polynome(int degre);
        polynome(std::vector<T> coefficients);

        //Accesseurs
        int get_degre() const;

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


////////////constructeurs///////////////////
template <typename T> polynome<T>::polynome(int degre)
:m_degre(degre)
{}

template <typename T> polynome<T>::polynome(std::vector<T> coefficients)
:m_coefficients( coefficients ), m_degre( coefficients.size() - 1 )
{}
        
//////////////Accesseurs/////////////////////
template <typename T> int polynome<T>::get_degre() const
{
    return m_degre;
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
    int m1( P.get_degre());
    int max(0); int min(0);
    m1 > m_degre ? max = m1 : max = m_degre ;
    m1 > m_degre ? min = m_degre : min = m1 ;

    int i=0;
    for(i; i<=min; i++)
    {
        m_coefficients[i]+=P[i];
    }
    if(m1==max)
    {
        i++;
        for(i; i<=max; i++)
        {
            m_coefficients.push_back(P[i]);
        }
        m_degre=m1;
    }
    return *this;
}

template <typename T> polynome<T>& polynome<T>::operator-=(const polynome<T>& P)
{
    int m1(P.get_degre());
    int max(0); int min(0);
    m1 > m_degre ? max = m1 : max = m_degre ;
    m1 > m_degre ? min = m_degre : min = m1 ;

    int i=0;
    for(i; i<=min; i++)
    {
        m_coefficients[i]-=P[i];
    }
    if(m1==max)
    {
        i++;
        for(i; i<=max; i++)
        {
            m_coefficients.push_back(-P[i]);
        }
        m_degre=m1;
    }
    return *this;
}   

template <typename T> polynome<T>& polynome<T>::operator*=(const polynome<T>& P)    //Multiplication par un polynome
{
    int m1=m_degre;
    int m2=P.get_degre();
    m_degre=m1+m2;

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
    for(int i=0; i<m_coefficients.size(); i++)
        m_coefficients[i]*= a;
    return *this;
}

template <typename T> polynome<T>& polynome<T>::operator/=(const T& a)              //Division par un scalaire
{
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

template<typename T> polynome<T> operator+(const polynome<T> &P,const polynome<T> &Q)
{
    polynome<T> res(P);
    return res+=Q;
}

template<typename T> polynome<T> operator-(const polynome<T> &P,const polynome<T> &Q)
{
    polynome<T> res(P);
    return res-=Q;
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

template<typename T>polynome<T> operator/(const polynome<T> &P,const T &a)
{
    polynome<T> res(P);
    return res/=a;    
}

template<typename T> std::ostream& operator<<(std::ostream& out , const polynome<T> &P)
{
    for(int i=0; i<=P.get_degre(); i++)
        out<<"rang "<<i<<" : "<< P[i] << std::endl;
    return out;
}



#endif