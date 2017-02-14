#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

class Parameter;
class Connector;

class Rshell{

protected:
vector< vector <Parameter*> >   v_lines;
vector<Connector>  v_connectors;
string  input;

public:
Rshell(){}                      // Default Constructor

void    read();                 // Loads input with cin, also exits if input is exit
void    parse();                // Organizes input into v_lines & v_connectors
void    execute();              // Execute v_lines according to v_connectors
                                // Does logic for v_connectors
};
class Parameter : public Rshell{

protected:
bool    usage = false;
string  parameter;

public:
Parameter(string s): usage(false), parameter(s){}

string  getParameter(); //
bool    getUsed();      // Returns usage
void    trueUsed();     // Change usage = true
void    falseUsed();    // Change usage = false
};


class Connector : public Rshell{

protected:
string  connector;

public:
Connector(string s): connector(s){}

string  getConnector(); // returns connector;
};

int main(){
    return 0;
}
