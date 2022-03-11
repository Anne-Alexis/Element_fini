#include "../header/arithmetique.hpp"
#include "../header/polynome.hpp"

    entier euclide(entier a, entier b)
    {
        entier r,c;
        if(a<b)
        {
            entier inter=a;
            a=b; b=inter;
        }
        r=a%b;
        c=b;
        while(r!=0)
        {
            b=r;
            r=c%r;
            c=b;
        }
        return c;
    }

    polynome<real> euclide(const polynome<real> &P,const polynome<real> &Q)
    {
        polynome<real> A(P);
        polynome<real> B(Q);
        if(A.get_degre()<B.get_degre())
        {
            polynome<real> INTER(A);
            A=B;
            B=INTER;
        }

        std::pair DIV(div_euclide(A,B));
        polynome<real> C(B);

        while(DIV.second.get_degre()>=0)
        {   
            B=DIV.second;
            DIV=div_euclide(C,DIV.second);
            C=B;
        }
        return C;
    }

    std::vector<entier> euclide_etendu(entier a, entier b)
    {
        entier u1=1;
        entier v1=0;
        entier u2=0;
        entier v2=1;
        entier u,v;

        if(a<b)
        {
            entier inter=a;
            a=b; b=inter;
        }
        entier q=a/b;
        entier r=a%b;
        u=u1-q*u2;
        v=v1-q*v2;   
        u1=u2;v1=v2;
        u2=u; v2=v;

        while(r!=0)
        {
            a=b;
            b=r;
            q=a/b;
            r=a%b;

            u=u1-q*u2;
            v=v1-q*v2;
            
            u1=u2;v1=v2;
            u2=u; v2=v;
        }

        return std::vector<entier>({b,u1,v1});
    }

    std::vector<polynome<real>> euclide_etendu(const polynome<real> &P, const polynome<real> &Q)
    {
        polynome<real> U1(std::vector<real>({1}));
        polynome<real> V1(std::vector<real>({0}));

        polynome<real> U2(std::vector<real>({0}));
        polynome<real> V2(std::vector<real>({1}));
        polynome<real> U(1);
        polynome<real> V(1);

        polynome<real> A(P);
        polynome<real> B(Q);

        if(A.get_degre()<B.get_degre())
        {
            polynome<real> INTER(A);
            A=B;
            B=INTER;
        }

        std::pair DIV(div_euclide(A,B));
        U=U1-DIV.first*U2;       
        V=V1-DIV.first*V2;
        U1=U2;
        V1=V2;
        U2=U; V2=V;

        while(DIV.second.get_degre()>=0)
        {   
            A=B;
            B=DIV.second;
            DIV=div_euclide(A,B);
            U=U1-(DIV.first*U2);
            V=V1-(DIV.first*V2);
            U1=U2;
            V1=V2;
            U2=U; V2=V;
        }
        return std::vector< polynome<real> > ({B,U1,V1}) ;
    }


    polynome<real> pade_n(const polynome<real>& S)
    {
        int n=S.get_degre();
        if(n%2==0)
        {
            std::cout<<"arth::pade_n Pb il faut que le degré soit impair"<<std::endl;
            exit(0);
        }
        int k=(n+1)/2;

        polynome<real> V1(std::vector<real>({0}));
        polynome<real> V2(std::vector<real>({1}));
        polynome<real> V(1);

        polynome<real> A(std::vector<real>({1}));
        A.decalage(n+1);
        polynome<real> B(S);

        std::pair DIV(div_euclide(A,B));      
        V=V1-DIV.first*V2;
        V1=V2;
        V2=V;

        while(DIV.second.get_degre()>=k)
        {   
            A=B;
            B=DIV.second;
            DIV=div_euclide(A,B);
            V=V1-(DIV.first*V2);
            V1=V2;
            V2=V;
        }
        return V2/(-1*V2[V2.get_degre()]);
    }


