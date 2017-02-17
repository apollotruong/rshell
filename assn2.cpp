#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;

class Rshell;
class Parameter;                // Pre-definition of Parameter
class Connector;                // Pre-definition of Connector

class Rshell{

protected:
public:
Rshell(){}                      // Default Constructor


};


class Parameter : public Rshell{

protected:
bool    usage;
string  parameter;

public:
Parameter(string s): usage(false), parameter(s){}

string  getParameter(){ // Returns string parameter
    return parameter;
}
bool    getUsed(){      // Returns usage
    return usage;
}
void    trueUsed(){     // Change usage = true
    usage = true;
}
void    falseUsed(){    // Change usage = false
    usage = false;
}
};


class Connector : public Rshell{

protected:
string  connector;

public:
Connector(string s): connector(s){}

string  getConnector(){ // returns connector;
    return connector;
}
};

class List : public Rshell{ // Composite
protected:
vector< vector <Rshell*> >   v_lines;
int lines_counter;
vector<Rshell*>  v_connectors;
string  input;


public:
List(){}
void    read(){                 // Loads input with cin, also exits if input is exit
    string read;
    cin >> read;                // Working on single commands first.
    while(cin >> read){
        if(read == "exit")
            exit(0);

        input += read;          // concat saved string <- typed string
        input += ' ';           // Separating with spaces
    }
}

void    parse(){                // Organizes input into v_lines & v_connectors
    stringstream ss(input);
    string temp;
    vector<Rshell*> v_temp;
    while(ss << temp){
        v_temp.push_back(new Parameter(temp));
    }

}

void    execute();              // Execute v_lines according to v_connectors
                                // Does logic for v_connectors
                                // forking and execbp will be handled here


};


int main(){
    /*
    Rshell* shello = new Rshell();
    
    while(1){
        shello->read();
        shello->parse();
        shello();execute();
    }
    */
    return 0;
}