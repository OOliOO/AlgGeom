#include "polynomial.hpp"
#include <sstream>
#include <algorithm>

Polynomial::Polynomial(int deg){
    coeffs.resize(deg+1);
}

Polynomial::Polynomial(vector<double> v){
    coeffs=v;
    reduce();
}

void Polynomial::reduce(){
    int hnz=coeffs.size()-1;
    while(hnz>=0&&abs(coeffs[hnz])<1e-9)
        hnz--;
    coeffs.resize(hnz+1);
}

string Polynomial::toString() const{
    if(coeffs.size()==0)
        return "0";

    string s="";

    for(int i=coeffs.size()-1; i>=0; i--){
        if(coeffs[i]==0)
            continue;
        if(s!="")
            s+=" + ";

        stringstream ss;
        ss<<coeffs[i];
        string tmp;
        ss>>tmp;

        if(i==0 || coeffs[i]!=1)
            s+=tmp;

        if(i==0)
            break;

        s+="x";

        if(i!=1){
            s+="^";
            stringstream ss2;
            ss2<<i;
            string exp;
            ss2>>exp;
            s+=exp;
        }
    }
    return s;
}

double Polynomial::coeff(int d) const{
    if(d>=coeffs.size())
        return 0;
    return coeffs[d];
}

int Polynomial::deg() const{
    return coeffs.size()-1;
}

bool Polynomial::operator ==(Polynomial g) const{
    return coeffs==g.coeffs;
}

Polynomial Polynomial::operator +(Polynomial g) const{
    Polynomial ret(max(this->deg(), g.deg()));
    for(int i=0; i<=max(this->deg(), g.deg()); i++)
        ret.coeffs[i]=this->coeff(i) + g.coeff(i);

    ret.reduce();

    return ret;
}

Polynomial Polynomial::operator -(Polynomial g) const{
    Polynomial ret(max(this->deg(), g.deg()));
    for(int i=0; i<=max(this->deg(), g.deg()); i++)
        ret.coeffs[i]=this->coeff(i) - g.coeff(i);

    ret.reduce();

    return ret;
}

Polynomial Polynomial::operator *(Polynomial g) const{
    if(this->deg()==-1 || g.deg()==-1)
        return Polynomial({0});

    int d=this->deg()+g.deg();
    Polynomial ret(d);

    for(int i=0; i<=d; i++)
        for(int e=0; e<=i; e++)
            ret.coeffs[i]+=this->coeff(i-e)*g.coeff(e);

    ret.reduce();   ///fucking zerodivisors...

    return ret;
}

Polynomial Polynomial::operator %(Polynomial g) const{
    if(g.deg()==0)
        return Polynomial({0});
    if(g.deg()==-1||g.deg()>this->deg())
        return (*this)+Polynomial({0});

    int d_h=this->deg()-g.deg();

    vector<double> v_h(d_h+1), v_f=coeffs, v_g=g.coeffs;

    int up=this->deg();
    double lead=*v_g.rbegin();

    for(int i=d_h; i>=0; i--){
        double c=v_f[up]/lead;
        v_h[i]=c;

        for(int e=0; e<=g.deg(); e++)
            v_f[up-e]-=c*v_g[g.deg()-e];

        v_f[up]=0;

        up--;
    }

    return Polynomial(v_f);
}

Polynomial Polynomial::operator /(Polynomial g) const{
    if(g.deg()==0)
        return Polynomial({0});
    if(g.deg()==-1||g.deg()>this->deg())
        return (*this)+Polynomial({0});

    int d_h=this->deg()-g.deg();

    vector<double> v_h(d_h+1), v_f=coeffs, v_g=g.coeffs;

    int up=this->deg();
    double lead=*v_g.rbegin();

    for(int i=d_h; i>=0; i--){
        double c=v_f[up]/lead;
        v_h[i]=c;

        for(int e=0; e<=g.deg(); e++)
            v_f[up-e]-=c*v_g[g.deg()-e];

        v_f[up]=0;

        up--;
    }

    return Polynomial(v_h);
}

Polynomial Polynomial::derivative() const{
    Polynomial ret(this->deg()-1);

    for(int i=0; i<=this->deg()-1; i++)
        ret.coeffs[i]=(i+1)*this->coeffs[i+1];
    return ret;
}

Polynomial GCD(Polynomial f, Polynomial g){
    if(f%g==Polynomial({0}))
        return g;
    return GCD(g,f%g);
}

bool isSeparable(Polynomial f){
    return GCD(f,f.derivative()).deg()==0;
}

ostream& operator<<(ostream& os, const Polynomial& f){
    os<<f.toString();
    return os;
}
