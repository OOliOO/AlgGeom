#ifndef MPOLY_ALG_HPP
#define MPOLY_ALG_HPP
#include "mpolynomial.hpp"

template<unsigned int N, typename COMP>
MPolynomial<N,COMP> remainder(MPolynomial<N,COMP> f, vector<MPolynomial<N,COMP>> v){
    MPolynomial<N,COMP> ret;

    while(!f.isZero()){
        bool canKill=0;
        for(MPolynomial<N, COMP> g:v)
            if(g.lead()<=f.lead()){
                canKill=1;

                MPolynomial<N,COMP> killer;
                killer.addMonomial(f.lead()-g.lead(), f.leadCoeff()/g.leadCoeff());
                f-=killer*g;
                break;
            }

        if(!canKill){
            ret.addMonomial(f.lead(),f.leadCoeff());
            f.killTerm(f.lead());
        }
    }
    return ret;
}

template<unsigned int N, typename COMP>
vector<MPolynomial<N,COMP>> calculateGroebnerBasis(vector<MPolynomial<N,COMP>> v){
    for(int i=0; i<v.size(); i++)
        for(int e=i+1; e<v.size(); e++){
            MPolynomial<N,COMP> a;
            a.addMonomial(LCM(v[i].lead(), v[e].lead()) - v[i].lead(), v[e].leadCoeff());

            MPolynomial<N,COMP> b;
            b.addMonomial(LCM(v[i].lead(), v[e].lead()) - v[e].lead(), v[i].leadCoeff());

            MPolynomial<N,COMP> s=a*v[i]-b*v[e];

            if(!remainder(s,v).isZero())
                v.push_back(s);
        }

    vector<MPolynomial<N,COMP>> v_min;

    for(int i=0; i<v.size(); i++){
        bool can_add=1;
        for(int e=i+1; e<v.size(); e++)
            if(v[e].lead()<=v[i].lead())
                can_add=0;
        if(can_add)
            v_min.push_back(v[i]);
    }
    return v_min;
}

#endif
