#ifndef UI_HPP
#define UI_HPP
#include <queue>
#include "mpolynomial.hpp"
#include "mpoly_alg.hpp"
#include "ideal.hpp"

#include <readline/readline.h>
#include <readline/history.h>

#define Pol MPolynomial<6, grlex<6>>
#define parse parse<6, grlex<6>>
#define Tuple Tuple<6>

map<string,string> type;
map<string,Pol> polynomials;

void _init(){
    type["x"]="polynomial";
    type["y"]="polynomial";
    type["z"]="polynomial";
    type["u"]="polynomial";
    type["v"]="polynomial";
    type["w"]="polynomial";

    polynomials["x"].addMonomial(Tuple({1,0,0,0,0,0}),1);
    polynomials["y"].addMonomial(Tuple({0,1,0,0,0,0}),1);
    polynomials["z"].addMonomial(Tuple({0,0,1,0,0,0}),1);
    polynomials["u"].addMonomial(Tuple({0,0,0,1,0,0}),1);
    polynomials["v"].addMonomial(Tuple({0,0,0,0,1,0}),1);
    polynomials["w"].addMonomial(Tuple({0,0,0,0,0,1}),1);
    
    type["quit"]="command";
    type["exit"]="command";
}

bool doubleChar(char c){
    return (c<='9'&&c>='0')||c=='.';
}

deque<string> breakIntoElements(string s){
    deque<string> q;

    for(int i=0; i<s.size(); i++){
        if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='('||s[i]==')'||s[i]==','||s[i]=='='||s[i]=='<'||s[i]=='>'||s[i]=='^'){
            q.push_back(s.substr(i,1));
            continue;
        }
        if(doubleChar(s[i])){
            string tmp="";
            while(i<s.size()&&doubleChar(s[i]))
                tmp+=s[i++];
            i--;
            q.push_back(tmp);
            continue;
        }
        if(s[i]<='z'&&s[i]>='a'){
            string tmp="";
            while(i<s.size()&&s[i]<='z'&&s[i]>='a')
                tmp+=s[i++];
            i--;
            q.push_back(tmp);
            continue;
        }
    }
    return q;
}

double toDouble(string s){
    stringstream ss;
    ss<<s;
    double ret;
    ss>>ret;
    return ret;
}

Pol evalP(deque<string> q){
    Pol ret;
    Pol term;
    term.addMonomial(Tuple({0,0,0,0,0,0}),1);
    bool term_exists=0;

    while(q.size()){
        if(q.front()=="+"){
            ret+=term;
            term=Pol();
            term.addMonomial(Tuple({0,0,0,0,0,0}),1);

            q.pop_front();
            term_exists=0;
            continue;
        }

        if(q.front()=="-"){
            ret+=term;
            term=Pol();
            term.addMonomial(Tuple({0,0,0,0,0,0}),-1);

            q.pop_front();
            term_exists=0;
            continue;
        }

        if(q.front()=="*"){
            q.pop_front();
            continue;
        }

        if(doubleChar(q.front()[0])){
            term_exists=1;

            Pol tmp;
            tmp.addMonomial(Tuple({0,0,0,0,0,0}), toDouble(q.front()));
            q.pop_front();

            if(q.size()&&q.front()=="^"){
                q.pop_front();
                if(!doubleChar(q.front()[0]))
                    throw "Error: the term following \'^\' is not even a number!";
                int exp=toDouble(q.front());
                q.pop_front();

                Pol tmp2;
                tmp2.addMonomial(Tuple({0,0,0,0,0,0}),1);

                for(int i=0; i<exp; i++){
                    tmp2=tmp2*tmp;
                }

                tmp=tmp2;
            }

            term=term*tmp;
            continue;
        }

        if(q.front()=="("){
            term_exists=1;
            deque<string> new_q;
            q.pop_front();
            int b_count=1;

            while(b_count&&q.size()){
                new_q.push_back(q.front());
                if(q.front()=="(")
                    b_count++;
                if(q.front()==")")
                    b_count--;
                q.pop_front();
            }

            if(b_count)
                throw "Malformed brackets!";

            if(new_q.size())
                new_q.pop_back();

            Pol tmp=evalP(new_q);

            if(q.size()&&q.front()=="^"){
                q.pop_front();
                if(!doubleChar(q.front()[0]))
                    throw "Error: the term following \'^\' is not even a number!";
                int exp=toDouble(q.front());
                q.pop_front();

                Pol tmp2;
                tmp2.addMonomial(Tuple({0,0,0,0,0,0}),1);

                for(int i=0; i<exp; i++){
                    tmp2=tmp2*tmp;
                }

                tmp=tmp2;
            }

            term=term*tmp;
            continue;
        }

        if(type[q.front()]=="polynomial"){
            term_exists=1;
            Pol tmp=polynomials[q.front()];
            q.pop_front();

            if(q.size()&&q.front()=="^"){
                q.pop_front();
                if(!q.size()||!doubleChar(q.front()[0]))
                    throw "Error: the term following \'^\' is not even a number!";

                int exp=toDouble(q.front());
                q.pop_front();

                Pol tmp2;
                tmp2.addMonomial(Tuple({0,0,0,0,0,0}),1);

                for(int i=0; i<exp; i++){
                    tmp2=tmp2*tmp;
                }

                tmp=tmp2;
            }

            term=term*tmp;
            continue;
        }

        throw "Unexpexted term: "+q.front();
    }
    //cout<<term<<" "<<term_exists<<endl;
    if(term_exists)
        ret+=term;
    return ret;
}

void startUI(){
    _init();

    while(1){
        
        string s;
        char* ln=readline("> ");
        add_history(ln);
        s=ln;
        s=rmSpace(s);

        deque<string> q=breakIntoElements(s);

        try{
            if(q.size()&&type[q.front()]=="command"){
                if(q.front()=="quit"){
                    cout<<"lol, ragequit..."<<endl;
                    return;
                }
                
                if(q.front()=="exit"){
                    cout<<"exiting..."<<endl;
                    return;
                }
            }
            
            if(q.size()){
                string tmp=q.front();
                q.pop_front();

                if(q.size()&&q.front()=="="){
                    q.pop_front();

                    if(q.size()&&q.front()=="<"){
                        type[tmp]="ideal";
                    }else{
                        type[tmp]="polynomial";
                        polynomials[tmp]=evalP(q);
                        cout<<tmp<<" = "<<polynomials[tmp]<<endl;
                    }
                }else{
                    q.push_front(tmp);
                    Pol p=evalP(q);
                    cout<<p<<endl;
                }
            }
        }catch(string err){
            cout<<err<<endl;
        }catch(const char* err){
            cout<<err<<endl;
        }
    }
}

#endif
