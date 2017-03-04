#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

/* Implementation of a Tree with Nodes
*
*/
bool treeo = false;
class Node{
    friend class Tree;
    private:
    string value;
    bool ran;
    Node* parent;
    Node* left;
    Node* right;
    public:
     Node* getRightChild(){
        return right;
    }
    Node* getLeftChild(){
        return left;
    }
    string getValue(){
        return value;
    }
    void createEmptyParent(){ // makes n empty parent of a node.
        if(parent){ // if a parent already exists, go up
            createEmptyParent(parent);
        }
        else{
            Node* nodo = new Node();    // make a new node
            parent = nodo;       // set parent
            nodo->left = this;         // set as left child
        }
    }
    void createEmptyParent(Node* nudes){ // makes n empty parent of a node.
        if(nudes->parent){ // if a parent already exists, go up
            createEmptyParent(nudes->parent);
        }
        else{
            Node* nodo = new Node();    // make a new node
            nudes->parent = nodo;       // set parent
            nodo->left = nudes;         // set as left child
        }
    }
    void createLeftChild(string value){ // with string value
        Node* nodo = new Node();
        parent->left = nodo;
        nodo->parent = parent;
    }

    void createRightChild(string value){ // with string value
        Node* nodo = new Node();
        parent->right = nodo;
        nodo->parent = parent;
    }

    void setNodeValue(string s){    // Sets Node value to S
        value = s;    //
    }
    void setLeftChild(Node* lhs, Node* rhs ){   // Sets left child and its parent
        lhs->parent = rhs;
        rhs->left = lhs;
    }
    void setRightChild(Node* lhs, Node* rhs){   // Sets right child and its parent
        lhs->parent = rhs;
        rhs->right = lhs;
    }
    void setRanFalse(Node* nodo){   // Boolean for if a node has been run.
        nodo->ran = false;
    }
    void setRanTrue(Node* nodo){   
        nodo->ran = true;
    }
    Node* getParent(){ 
        return parent;
    }

};

/*  Tree uses Nodes.
*   Puts the arguments and connectors in a tree format to solve for precedence
*/
class Tree{
    private: 
    Node *root;

    public:
    Tree(){
        Node* nodo = new Node;
        root = nodo;
    }
    ~Tree(){}
    Node* getRoot(){
        return root;
    }
    void setRoot(Node* nodo){
        root = nodo;
    }
};

/*  PRE-DEFINITION OF RSHELL CLASSES AS WELL AS THE IMPLEMENTATION
*
*
*
*/
class Connector;                // Pre-definition of Connector
class List;						// Pre-definition of List
class Parameter;                // Pre-definition of Parameter
class Rshell;                   // Pre-definition of Rshell
int vl_it;          //global v_lines iterator
int numcon;

class Rshell{
public:
Rshell(){}
virtual ~Rshell(){};
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
vector<Parameter*> v_lines;             // Vector of Commands
vector<Connector*> v_connectors;        // Vector of connectors
int lines_counter;                      // counts how many lines in v_lines
string  input;                          // Whole input, (includes connectors)

public:
List(): lines_counter(0), input(""){}

void parse(string& read){                           // Organizes input into v_lines & v_connectors
    input = read;
    if(getInputLength() == 0){          // Input length should not be 0
        return;
    }
    int whitespace = 0;
    while(input.at(whitespace)  == ' '){   // Handle all leading whitespace
        whitespace++;
    }
    input = input.substr(whitespace);
		int p_counter = 0;
    for(unsigned i = 0; i < input.size(); i++){                 // Handle ';'
        if(input.at(i) == ';'){
            v_connectors.push_back(new Connector(";"));
            numcon++;
        }
        else if(input.at(i) == '&'){
            if(input.at(i+1) == '&'){
                v_connectors.push_back(new Connector("&&"));    // Handle "&&"
                numcon++;
            }
        }
        else if(input.at(i) == '|'){
            if(input.at(i+1) == '|'){
                v_connectors.push_back(new Connector("||"));    // Handle "||"
                numcon++;
            }
        }
        else if(input.at(i) == '#'){                // If its a comment, do nothing
            input = input.substr(0,i);
            break;
        }
				else if(input.at(i) == '('){
					v_connectors.push_back(new Connector("(")); // check for a (
					p_counter++;
				}
				else if(input.at(i) == ')'){
					v_connectors.push_back(new Connector(")")); // check for a )
					p_counter--;
				}
				else if(input.at(i) == '['){
					for(unsigned j = 0; j < input.size(); j++){ // search for ]
						if(input.at(j) == ']'){
							input.replace(j,1,"");				// replaces [] bounds with test
							input.replace(i,1,"test ");
							break;
						}
					}
						if(input.at(i) == '['){
							cout << "Some error with [].";
							return;
						}
					}

    }
        /*
        *   LOGIC TO CHECK IF THERE ARE AN ODD AMOUNT OF PARENTHESIS
        */
		if(p_counter != 0){ // missing a )
			cout << "There was a parenthesis error () !\n";
			return;
		}
    /*
    *   Removal of connectors from the string before parsing into argument list
    */
    char* input_c = (char*)input.c_str();
    char* tok = strtok(input_c, ";|&#()");
    while(tok != NULL){
        string temp = tok;
        whitespace = 0;                 // Removing whitespace
        while (temp.at(whitespace) == ' '){
            whitespace++;
        }
        temp = temp.substr(whitespace);
        v_lines.push_back(new Parameter(temp));
        tok = strtok(NULL, ";|&#()");
    }

    /*  Check case for singular command
    *    if so, run like normal
    */
	for(unsigned i = 0; i < v_connectors.size(); i++){
        if(v_connectors.at(i)->getConnector() == "("){
            if(v_connectors.at(i+1)->getConnector() == ")"){
                v_connectors.erase(v_connectors.begin()+i);
                v_connectors.erase(v_connectors.begin()+i);
                treeo = false;
            }
        }
    }

}
/*
*   PRINTS v_lines AND v_connectors USED FOR DEBUGGING
*/
void print(){
    cout << "______________________________" << endl << "v_lines" << endl;
    for(unsigned i = 0; i < v_lines.size(); i++){
        cout << "index " << i << ": " << v_lines.at(i)->getParameter() << endl;
    }
    cout << endl << "v_connectors:" << endl;
    for(unsigned i = 0; i < v_connectors.size(); i++){
        cout << "index " << i << ": " << v_connectors.at(i)->getConnector() << endl;
    }
		cout << "______________________________" << endl;
}

/*
*   Runs before execute to see which commands will be run
*/
void checkexecute(int endindex){
  // if there are no connectors, dont delete anything
  if(v_connectors.size() == 0){ return; }
  // if there are connectors, start checking which ones to delete
  else{
    //always set the very first argument as used
    v_lines.at(0)->trueUsed();
    int finishcounter = 0;    // keeps count for when to stop

    //check logic till v connectors is empty, or hits a paranthesis end
    while(v_connectors.size() > 0 || finishcounter < endindex ){

    //   // deleting instances of ')'
    //   if(v_connectors.at(0)->getConnector() == ")"){
    //     v_connectors.erase(v_connectors.begin());
    //   }

    //   // deleting instances of '('
    //   if(v_connectors.at(0)->getConnector() == "("){
    //       v_connectors.erase(v_connectors.begin());
    //   }


      // execute arguments following ';'
      if(v_connectors.at(0)->getConnector() == ";"){
        vl_it++;
        v_lines.at(vl_it)->trueUsed(); // mark as used
        v_connectors.erase(v_connectors.begin()); //erase ;
        finishcounter++;
      }

      // logic for ||
      else if(v_connectors.at(0)->getConnector() == "||"){
        if(!v_lines.at(vl_it)->getUsed()){  //only run if last thing wasnt
          vl_it++;
          v_lines.at(vl_it)->trueUsed(); // mark as used
        }
        else{
          v_lines.erase(v_lines.begin() + vl_it + 1); //erase unused line
        }
        v_connectors.erase(v_connectors.begin()); // erase ||
        finishcounter++;
      }

      // logic for &&
      else if(v_connectors.at(0)->getConnector() == "&&"){
        if(v_lines.at(vl_it)->getUsed()){ //only run if last thing was
          vl_it++;
          v_lines.at(vl_it)->trueUsed(); // mark as used
        }
        else{
          v_lines.erase(v_lines.begin() + vl_it + 1); //erase unused line
        }
        v_connectors.erase(v_connectors.begin()); // erase &&
        finishcounter++;
      }
    }
  }

  return;
}

/*
*   void execute shouldnt need to look at v_connectors at all
*   Will do logic to use execvp to use commands to terminal
*/  
void execute(){

  for(int i = 0; i < v_lines.size(); i++){
    //the following parses each line into seperate words into command[]
		stringstream ss(v_lines.at(i)->getParameter());
		istream_iterator<string> begin(ss);
		istream_iterator<string> end;
		vector<string> vstrings(begin, end);
		vector<char *> commandVector;
		for(int x = 0; x < vstrings.size(); x++){
			char *temp = new char[vstrings.at(x).length() + 1];
			strcpy(temp, vstrings.at(x).c_str());
			commandVector.push_back(temp);
		}
		commandVector.push_back(NULL);
		char **command = &commandVector[0];

    // cout << "commandVector[0] : " << commandVector[0] << endl;
    // cout << "commandVector[1] : " << commandVector[1] << endl;
    // cout << "commandVector[2] : " << commandVector[2] << endl;
    // test(commandVector[1], commandVector[2]);
    string teststring = commandVector[0];

    if(teststring == "exit"){                 // Exit conditional
            cout << "\nExiting...\n";
            exit(0);                    // Exit
    }
    else if(teststring == "test"){ // if the command is test, run test()
      test(commandVector[1], commandVector[2]);
    }
    else{ // if not test, run execvp
      // using forking and execvp to execute each seperately parsed argument
  		pid_t pid;
  		int status;
  		if((pid = fork()) < 0){
  			cout << "Error: failed to fork child process" << endl;
  			exit(1);
  		} 
  		else if(pid == 0){
  			if(execvp(command[0], command) < 0){
  				cout << "Error: failed to execute" <<endl;
  				exit(1);
  			}
  		}
  		else{
  			while(wait(&status) != pid); // wait for parent
  		}
    }
  }
  return;
}

/* Functon that prints (TRUE) or (FALSE) depending on the existence of a file / directory
*		takes in a flag:	 "", "-e", "-f", or "-d"
*		also takes in path of file/directory
*/
void test(const string& flags, const string& path_name){
	char* path_name_c = (char*)path_name.c_str();
	struct stat s;

	if(flags == "-e" || flags == ""){ // checks for -e
		if(stat(path_name_c, &s) == 0){ // if this file exists
			cout << "(TRUE)" << endl;
			return;
		}
		else{ // otherwise false;
		cout << "(FALSE)" << endl;
		return;
		}
	}
	else if(flags == "-f"){ // checks for -f
		stat(path_name_c, &s); // if this file is a regular file
		if(S_ISREG(s.st_mode)){
			cout << "(TRUE)" << endl;
			return;
		}
		else{ // otherwise false;
		cout << "(FALSE)" << endl;
		return;
		}
	}
	else if(flags == "-d"){ // checks for -d
		stat(path_name_c, &s); // if this file is a directory
		if(S_ISDIR(s.st_mode)){
			cout << "(TRUE)" << endl;
			return;
		}
		else{ // otherwise false;
		cout << "(FALSE)" << endl;
		return;
		}
	}
}

int getInputLength(){           // Returns input.length();
    return input.length();
}
/*
*   Create a tree to solve the logic of the commands with precedence operators ( )
*   After this is done executing, run executeTree();
*/
void createTree(){
    cout << "debug" << endl;
    int i = 0;
    int j = 0;
    Tree tree;
    Node* curr = tree.getRoot();
    /*
    *   Logic to create a tree with the v_lines and v_connectors
    */
    while(j < v_connectors.size()){
        if(v_connectors.at(i)->getConnector() == "("){
            if(curr->getValue() == ""){
                curr->createLeftChild(v_lines.at(j)->getParameter()); // make left child
                j++; i++;   // increment counter for the v_lines and v_connectors
                curr->setNodeValue(v_connectors.at(i)->getConnector());
                i++;
                curr->createEmptyParent();
                curr->createRightChild("");
                curr = curr->getRightChild();
            }
            else{
                curr = curr->getRightChild();
            }    
        }
        
        // If the value is a connector...
        if(v_connectors.at(i)->getConnector() == "&&"
        || v_connectors.at(i)->getConnector() == "||"
        || v_connectors.at(i)->getConnector() == ";"){
            curr->setNodeValue(v_connectors.at(i)->getConnector());
            i++;
            if(curr->getLeftChild() == NULL){ // if no left child, make one
                curr->createLeftChild(v_lines.at(j)->getParameter());
                j++;
                curr->createRightChild("");
                curr = curr->getRightChild();
            }
            else{
                curr->createRightChild(""); // if no right child, make one
                curr = curr->getRightChild();
            }
        }

        if(v_connectors.at(i)->getConnector() == ")"){
            curr->setNodeValue(v_lines.at(j)->getParameter());
            j++;
            while(curr->getParent()){
                curr = curr->getParent();
            }
        }
    }
    // if NULL
    if(curr->getValue() == ""){
        curr = curr->getLeftChild();
        delete curr->getParent();
        tree.setRoot(curr);
    }
    executeTree(curr);
}

/*
*   Execute the tree 
*/
void executeTree(Node* top){
    Node* curr = top;
    if(curr->getValue() == ";" || curr->getValue() == "&&"){
        if(curr->getLeftChild())
            executeTree(curr->getLeftChild());
        if(curr->getRightChild())
            executeTree(curr->getRightChild());

    }
    if(curr->getValue() == "||"){
        executeTree(curr->getLeftChild());
    }
    
    /*  Make a command vector of all the commands to run in the shell
    *   print stuff out to execvp through the command vector afterwards
    */
    else{
        stringstream ss(curr->getValue());
		istream_iterator<string> begin(ss);
		istream_iterator<string> end;
		vector<string> vstrings(begin, end);
		vector<char *> commandVector;
		for(int x = 0; x < vstrings.size(); x++){
			char *temp = new char[vstrings.at(x).length() + 1];
			strcpy(temp, vstrings.at(x).c_str());
			commandVector.push_back(temp);
		}
		commandVector.push_back(NULL);
		char **command = &commandVector[0];

    // cout << "commandVector[0] : " << commandVector[0] << endl;
    // cout << "commandVector[1] : " << commandVector[1] << endl;
    // cout << "commandVector[2] : " << commandVector[2] << endl;
    // test(commandVector[1], commandVector[2]);
    string teststring = commandVector[0];

    if(teststring == "exit"){                 // Exit conditional
            cout << "\nExiting...\n";
            exit(0);                    // Exit
    }
    else if(teststring == "test"){ // if the command is test, run test()
      test(commandVector[1], commandVector[2]);
    }
    else{ // if not test, run execvp
      // using forking and execvp to execute each seperately parsed argument
  		pid_t pid;
  		int status;
  		if((pid = fork()) < 0){
  			cout << "Error: failed to fork child process" << endl;
  			exit(1);
  		} 
  		else if(pid == 0){
  			if(execvp(command[0], command) < 0){
  				cout << "Error: failed to execute" <<endl;
  				exit(1);
  			}
  		}
  		else{
  			while(wait(&status) != pid); // wait for parent
  		}
    }
    }
}
}; // End of List class

string read(){                      	  	// Loads input with cin, also exits if input is exit
    string input;
    cout << getenv("PWD") << " $ ";     // Prints working dir
    getline(cin, input);                 // Take whatever they input on a line
    return input;
}

int main(){
    while(1){   // Keep looping
        List* shello = new List();  // Make new instance
        string poop_input = read();             // Read in prompt and read input
        if(poop_input.length() == 0){} // Empty input -> re-loop
        else{                               // Input is OK
            
            numcon = 0;

            /*
            *    if there is a (, run logic with tree
            */
            for(unsigned i = 0; i < poop_input.length(); i++){  
                if(poop_input.at(i) == '('){                    
                    treeo = true;
                }
            }
            shello->parse(poop_input);                // Parse

            if(treeo){
                shello->createTree();       // Tree solving for ( ) precedence
            }
            else{
                // shello->print();             // Print v_lines and v_connectors; (USED FOR DEBUGGING)
                vl_it = 0;
                shello->checkexecute(numcon);
                shello->execute();              // Execute
            }
        }
        treeo = false;
        delete shello;                      // Goodbye shello :-(
    }
    return 0;
}
