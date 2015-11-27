#ifndef MPOLYNOMIAL_HPP
#define MPOLYNOMIAL_HPP
#include <map>
#include "tuple.hpp"
/*
Polynomials of finitely many variables over "reals".

Structure:
class MPolynomial<N,COMP>{
    MPolynomial(){}
    MPolynomial(vector<pair<Tuple<N>, double>>);
    vector<pair<Tuple<N>, double>> _rawData();

    void addMonomial(Tuple<N>, double);

    Tuple<N> lead() const;

    void killTerm(Tuple<N>);

    MPolynomial operator +(MPolynomial) const;
    MPolynomial operator -(MPolynomial) const;
    MPolynomial operator *(MPolynomial) const;

    MPolynomial operator +=(MPolynomial);
    MPolynomial operator -=(MPolynomial);

    bool operator ==(MPolynomial) const;

    string toString() const;
    bool isZero() const;
}

ostream& operator<<(ostream&, const MPolynomial<N,COMP>&);
MPolynomial<N,COMP> parse(string);
*/

template<unsigned int N, typename COMP>
class MPolynomial{
    map<Tuple<N>, double, COMP> data;

    void reduce(){
        vector<Tuple<N>> toBeRemoved;
        for(auto a:data)
            if(abs(a.second())<1e-9)
                toBeRemoved.push_back(a.first);
        for(auto key:toBeRemoved)
            data.erase(key);
    }

public:
    MPolynomial(){}

    MPolynomial(vector<pair<Tuple<N>, double>> v){
        for(auto a:v)
            data[v.first]=v.second;
    }

    vector<pair<Tuple<N>, double>> _rawData(){
        vector<pair<Tuple<N>,double>> v;
        for(auto a:data)
            v.push_back(*a);
        return v;
    }

    void addMonomial(Tuple<N> t, double coeff){
        data[t]+=coeff;
        if(abs(data[t])<1e-9)
            data.erase(t);
    }

    Tuple<N> lead() const{
        return (*data.rbegin()).first;
    }

    double leadCoeff() const{
        return (*data.rbegin()).second;
    }

    void killTerm(Tuple<N> t){
        data.erase(t);
    }

    MPolynomial operator +(MPolynomial b) const{
        MPolynomial ret;
        ret.data=data;
        for(auto a:b.data){
            ret.data[a.first]+=a.second;
            if(abs(ret.data[a.first])<1e-9)
                ret.data.erase(a.first);
        }
        return ret;
    }

    MPolynomial operator -(MPolynomial b) const{
        MPolynomial ret;
        ret.data=data;
        for(auto a:b.data){
            ret.data[a.first]-=a.second;
            if(abs(ret.data[a.first])<1e-9)
                ret.data.erase(a.first);
        }
        return ret;
    }

    MPolynomial operator *(MPolynomial b) const{
        MPolynomial ret;
        for(auto a1:data)
            for(auto a2:b.data){
                ret.data[a1.first + a2.first]+=a1.second*a2.second;

                if(abs(ret.data[a1.first + a2.first])<1e-9)
                    ret.data.erase(a1.first + a2.first);
            }
        return ret;
    }

     MPolynomial& operator +=(MPolynomial b){
        for(auto a:b.data){
            data[a.first]+=a.second;

            if(abs(data[a.first])<1e-9)
                data.erase(a.first);
        }
        return *this;
    }

    MPolynomial operator -=(MPolynomial b){
        for(auto a:b.data){
            data[a.first]-=a.second;

            if(abs(data[a.first])<1e-9)
                data.erase(a.first);
        }
        return *this;
    }

    MPolynomial& operator =(MPolynomial b){
        data=b.data;
        return *this;
    }

    bool operator ==(MPolynomial b) const{
        return ((*this)-b).data.size()==0;
    }

    string toString() const{
        string s="";
        for(auto a=data.rbegin(); a!=data.rend(); a++){
            if(s!=""){
                if(a->second>=0)
                    s+=" + ";
                else
                    s+=" - ";
            }

            if(s==""&&a->second<0)
                s+="-";

            if(abs(a->second)!=1 || a->first.isConst()){
                stringstream ss;
                ss<<abs(a->second);
                string c;
                ss>>c;
                s+=c;
            }

            if(!a->first.isConst())
                s+=a->first.toString();
        }
        if(s=="")
            return "0";
        return s;
    }

    bool isZero() const{
        return data.size()==0;
    }
};

template<unsigned int N, typename COMP>
ostream& operator<<(ostream& os, const MPolynomial<N,COMP>& f){
    os<<f.toString();
    return os;
}

inline string rmSpace(string s){
    string ret="";
    for(int i=0; i<s.size(); i++)
        if(s[i]!=' ')
            ret+=s[i];
    return ret;
}

inline bool _valid(string s, int it){
    return ((s[it]<='9'&&s[it]>='0')||s[it]=='.');
}

inline double _extract_next_double(string s, int& it){
    if((!_valid(s,it)&&s[it]!='+'&&s[it]!='-')||it>=s.size())
        return 1;

    double sgn=1;

    if(s[it]=='-')
        sgn=-1,it++;
    if(s[it]=='+')
        sgn=1,it++;

    string tmp="";
    while(it<s.size()&&_valid(s, it)){
        tmp+=s[it];
        it++;
    }

    if(tmp=="")
        return sgn;

    stringstream ss;
    ss<<tmp;
    double ret;
    ss>>ret;

    return sgn*ret;
}

template<unsigned int N>
inline Tuple<N> _extract_next_tuple(string s, int& it){
    int _rev_var[256];
    for(int i=0; i<256; i++)
        _rev_var[i]=-1;

    _rev_var['x']=0;
    _rev_var['y']=1;
    _rev_var['z']=2;
    _rev_var['u']=3;
    _rev_var['v']=4;
    _rev_var['w']=5;

    if(it>=s.size() || _rev_var[s[it]]==-1)
        return Tuple<N>();

    Tuple<N> ret;
    while(it<s.size() && _rev_var[s[it]]!=-1){
        int v=_rev_var[s[it]];
        it++;
        int exp=1;
        if(it<s.size()&&s[it]=='^'){
            it++;
            exp=round(_extract_next_double(s,it));
        }
        ret.data[v]+=exp;

        if(it<s.size()&&s[it]=='*')
            it++;
    }
    return ret;
}

template<unsigned int N, typename COMP>
MPolynomial<N,COMP> parse(string s){
    MPolynomial<N,COMP> ret;

    int it=0;
    s=rmSpace(s);
    while(it<s.size()){
        double c=_extract_next_double(s, it);
        Tuple<N> t=_extract_next_tuple<N>(s, it);

        ret.addMonomial(t,c);
    }

    return ret;
}

#endif

