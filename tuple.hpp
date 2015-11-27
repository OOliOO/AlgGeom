#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <initializer_list>
#include <iostream>
#include <sstream>

using namespace std;

/*
N-Tuples of integers, used to represent the basis
for multivariate polynomials.

Structure:

class Tuple<N>{
public:
    Tuple();
    Tuple(initializer_list<unsigned int>);
    inline unsigned int& operator [](unsigned int);
    inline const unsigned int& operator [](unsigned int) const;
    string toString() const;
    bool isConst() const;

    Tuple<N> operator+(const Tuple<N>) const;
    Tuple<N> operator-(const Tuple<N>) const;
    bool operator<=(const Tuple<N>) const;    a<=b if a divides b
};

ostream& operator<<(ostream&, const Tuple<N>&);
unsigned int abs(Tuple<N>);
Tuple<N> LCM(Tuple<N>, Tuple<N>);

class lex;
class grlex;
class grevlex;
*/

const string _var[]={"x", "y", "z", "u", "v", "w"};

template<unsigned int N>
class Tuple{
public:
    unsigned data[N];

    Tuple(){
        for(int i=0; i<N; i++)
            data[i]=0;
    }

    Tuple(initializer_list<unsigned int> list){
        int it=0;
        if(list.size()!=N)
            cout<<"Wrong number of arguments given in the initialization of N-tuple!\n";
        else
            for(int i:list)
                data[it++]=i;
    }

    inline unsigned int& operator [](unsigned int i){
        return data[i];
    }

    inline const unsigned int& operator [](unsigned int i) const{
        return data[i];
    }

    string toString() const{
        string s="";
        for(int i=0; i<N; i++){
            if(data[i]==0)
                continue;

            if(s!="")
                s+="*";

            #if N>6
                s+="x_";

                stringstream ss;
                ss<<i+1;
                string n;
                ss>>n;
                s+=n;
            #endif

            #if N<=6
                s+=_var[i];
            #endif

            if(data[i]!=1){
                s+="^";
                stringstream ss2;
                ss2<<data[i];
                string e;
                ss2>>e;
                s+=e;
            }
        }
        if(s=="")
            return "1";
        return s;
    }

    bool isConst() const{
        for(int i=0; i<N; i++)
            if(data[i]!=0)
                return false;
        return true;
    }

    Tuple<N> operator+(const Tuple<N> b) const{
        Tuple ret;
        for(int i=0; i<N; i++)
            ret[i]=data[i]+b[i];
        return ret;
    }

    Tuple<N> operator-(const Tuple<N> b) const{
        Tuple ret;
        for(int i=0; i<N; i++)
            ret[i]=data[i]-b[i];
        return ret;
    }

    bool operator<=(const Tuple<N> b) const{
        for(int i=0; i<N; i++)
            if(data[i]>b[i])
                return false;
        return true;
    }
};

template<unsigned int N>
ostream& operator<<(ostream& os, const Tuple<N>& t){
    os<<t.toString();
    return os;
}

template<unsigned int N>
unsigned int abs(Tuple<N> t){
    int s=0;
    for(auto a:t.data)
        s+=a;
    return s;
}

template<unsigned int N>
Tuple<N> LCM(Tuple<N> a, Tuple<N> b){
    Tuple<N> ret;
    for(int i=0; i<N; i++)
        ret[i]=max(a[i],b[i]);
    return ret;
}

template<unsigned int N>
class lex{
public:
    bool operator()(Tuple<N> a, Tuple<N> b){
        for(int i=0; i<N; i++){
            if(a[i]<b[i])
                return true;
            if(a[i]>b[i])
                return false;
        }
        return false;
    }
};

template<unsigned int N>
class grlex{
public:
    bool operator()(Tuple<N> a, Tuple<N> b){
        if(abs(a)==abs(b)){
            for(int i=0; i<N; i++){
                if(a[i]<b[i])
                    return true;
                if(a[i]>b[i])
                    return false;
            }
            return false;
        }
        return abs(a)<abs(b);
    }
};

template<unsigned int N>
class grevlex{
public:
    bool operator()(Tuple<N> a, Tuple<N> b){
        if(abs(a)==abs(b)){
            for(int i=N-1; i>=0; i--){
                if(a[i]>b[i])
                    return true;
                if(a[i]<b[i])
                    return false;
            }
            return false;
        }
        return abs(a)<abs(b);
    }
};

#endif
