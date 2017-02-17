#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;

class Rshell;
class Parameter;                // Pre-definition of Parameter
class Connector;                // Pre-definition of Connector

class Rshell{
public:
virtual void read();
virtual void parse();
virtual void execute();
virtual ~Rshell();
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

string  getConnector(){                 // returns connector;
    return connector;
}
};

class List : public Rshell{             // Composite
protected:
vector< vector <Rshell*> >   v_lines;   // Vector of Commands
vector<Rshell*>  v_connectors;          // Vector of connectors
int lines_counter;                      // counts how many lines in v_lines
string  input;                          // Whole input, (includes connectors)


public:
List(): lines_counter(0), input(""){}
void    read(){                         // Loads input with cin, also exits if input is exit
    cout << getenv("PWD") << " $ ";     // Prints working dir.
    string read;                        
    getline(cin, read);                 // Take whatever they input on a line
    if(input == "exit"){                // Exit conditional
            cout << "\nExiting...\n";   
            exit(0);                    // Exit
    }
    else{input = read;}                 // Sets class private to read
}

void    parse(){                        // Organizes input into v_lines & v_connectors
    stringstream ss(input);
    string temp;
    vector<Rshell*> v_temp;
    while(ss << temp){
        for(int i = 0; i < temp.length(); i++){
            if(temp.at(i) == ';'){
                
                v_temp.push_back(new Connector(temp));
            }
            if(temp.at(i) == '#'){
                
            }
            if(temp.at(i) == '&'){
                if(temp.at(i-1) == '&'){

                }
            }
            if(temp.at(i) == '|'){
                if(temp.at(i-1) == '|'){

                }
            }
        }
    }
}

void    execute();              // Execute v_lines according to v_connectors
                                // Does logic for v_connectors
int getInputLength(){           // Returns input.length();
    return input.length();
}
};


int main(){
    while(1){   // Keep looping
        List* shello = new List();  // Make new instance
        shello->read();             // Read in prompt and read input
        if(shello->getInputLength() == 0){} // Empty input -> re-loop
        else{                               // Input is OK
            shello->parse();                // Parse
            shello->execute();              // Execute
        }
        delete shello;                      // Goodbye shello :-(
    }
    return 0;
}