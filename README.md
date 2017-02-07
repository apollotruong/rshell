# rshell

Feb 06, 2017
>Complete rehaul of the skeleton code:

```

class Rshell {
public:
Rshell(){}

void readInput(){}
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

void parse(){
}

void execute(){
}
virtual string getType() = 0;
};



int main(){
return 0;
}

```

Everything still subject to change. Will continue tomorrow with you.
