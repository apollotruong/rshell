#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

class Rshell {
public:
Rshell(){}
//Default constructor
void readInput(){
    string s;
    cin >> s;
}
//Will take in inputs and send them to populateList().

void populateList(){}
//Will create the List of leaves
void exit();
//Will stop execution.
virtual string getType() = 0;
};

//Anything leading with a # will be considered a comment, until a newline
class Comment : public Rshell{
private:
string comment;

public:
Comment(){ };   //Default Constructor
Comment(string cmnt){
    comment = cmnt;
}      //Initializing
string getType(){
    return comment;
}

};

/* Handle the command. e.g. “ls”, “mkdir”, …
Given v.at(<any_number>).at(0) will be a Command */
class Command{
private:
char* command;

public:
Command(char* cmd) : command(cmd) { }
string getType(){
    return command;
}
};

class Argument: public Rshell{
private:
char* argument;

public:
Argument(char* arg) : argument(arg){}
string getType(){
    return argument;
}
    //Handle anything with the suffix “-” e.g. “-a” “-i” “-v”
};

/*  Handle “||”, “&&”, and “;” connectors   */
class Connector : public Rshell{
private:
string connector;

public:
Connector(string cnct): connector(cnct){ }
string getType(){
    return connector;
}
};

class List: public Rshell{
protected:
vector<Rshell*> v_rshell; //commands
vector<Rshell*> v_connector; // connectors;
string input;
public:
List(){}
//Default constructor

void print(vector<Rshell*> &v){   // will print the vec<rshell*>
    for(unsigned i = 0; i < v.size(); i++){
        cout << v.at(i)->getType();
        cout << endl;
    }
}

// void populateList(string s){    // takes in a line and pushes to 1dimension
//     string item;
//     getline(cin, item);
//     v.at(counter).push_back(s);
// }

void parse(){
    if(input == "") return; // if input is empty

    int counter = 0;
    while(input.at(counter) == ' '){    //used to skip over whitespace
        counter++;
    }
    input = input.substr(counter);    /* cut input at space */
    if(input.at(0) == '#'){
        v_rshell.push_back(new Comment("#"));
        return;
    }

    /* HANDLE ALL THE CONNECTORS FIRST */
    for(unsigned i = 0; i < input.length(); i++){
        if(input.at(i) == '#')          v_connector.push_back(new Connector("#"));
        if(input.at(i) == ';')          v_connector.push_back(new Connector(";"));
        if(input.at(i) == '&')
            if(input.at(i+1) == '&')    v_connector.push_back(new Connector("&&"));
        if(input.at(i) == '|')
            if(input.at(i+1) == '|')    v_connector.push_back(new Connector("&&"));
    }
    if(numParenthese != 0){
        cout << "Something went wrong with your parentheses\n";
        return;
    }
    char* cs = (char*)input.c_str(); //convert string to cstr
    char* tok = strtok(cs, "#;|&()"); //skip these cases
    while(tok != NULL){ //not space
        v_rshell.push_back(new Argument(tok)); // push back arguments
        tok = strtok(NULL, ";#|&()"); // skip these cases again
    }

}

void execute(){

}
virtual string getType() = 0;
};



int main(){
    return 0;
}
