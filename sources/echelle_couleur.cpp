#include "../header/echelle_couleur.hpp"

sf::Color rvb(real x, real min, real max)
{
    int r,v,b;
    if (x < min)
    {
        r=0;
        v=0;
        b=255;
    }
    else if (x > max)
    {
        r=255;
        v=0;
        b=0;
    }
    else if(min==max)
    {
        r=128;
        v=128;
        b=128;
    }
    else
    {
        int c= (int) 255*(x-min)/(max-min);
        c<128 ? b = 255-2*c : b=0;
        c>128 ? r = 2*c-255 : r=0;
        if(c==0 || c==255)
        {
            v=0;
        }
        else
        {
            v =  255 - 2*std::abs(c - 128);
        }
    }

    return sf::Color(r,v,b);
}

sf::Color noir_blanc(real x, real min, real max)
{
    int c;
    if (x < min)
        c= 0;
    if (x > max)
        c= 255;
    if(min==max)
        c= 128;

    c= (int) 255*(x-min)/(max-min);
    return sf::Color(c,c,c);
}