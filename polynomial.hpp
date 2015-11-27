#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP
#include <iostream>
#include <vector>
#include <initializer_list>
using namespace std;

/*
Polynomials of one variable in "real" coefficients.
The grading is standard, with the exception of deg(0)=-1.
*/

class Polynomial{
private:
    vector<double> coeffs;
    Polynomial(int deg);
    void reduce();
public:
    template<typename T> Polynomial(initializer_list<T> list){
        coeffs.resize(list.size());

        int i=list.size()-1;
        for(T t:list){
            coeffs[i]=t;
            i--;
        }

        reduce();
    }

    Polynomial(vector<double>);

    string toString() const;
    double coeff(int) const;
    int deg() const;

    bool operator ==(Polynomial g) const;

    Polynomial operator +(Polynomial g) const;
    Polynomial operator -(Polynomial g) const;
    Polynomial operator *(Polynomial g) const;
    Polynomial operator %(Polynomial g) const;
    Polynomial operator /(Polynomial g) const;

    Polynomial derivative() const;

};

ostream& operator<<(ostream& os, const Polynomial& f);
Polynomial GCD(Polynomial, Polynomial);
bool isSeparable(Polynomial);

#endif
