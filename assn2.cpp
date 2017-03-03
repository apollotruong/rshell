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

/**
*/

using namespace std;

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
void read(){                      	  	// Loads input with cin, also exits if input is exit
    string read;
    cout << getenv("PWD") << " $ ";     // Prints working dir
    getline(cin, read);                 // Take whatever they input on a line
    if(read == "exit"){                 // Exit conditional
            cout << "\nExiting...\n";
            exit(0);                    // Exit
    }
    else{input = read;}                 // Sets class private to read
}

void parse(){                           // Organizes input into v_lines & v_connectors
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
            return;
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
					v_connectors.push_back(new Connector("[")); // if it's a call for [test]
					for(unsigned j = 0; j < input.size(); j++){ // search for ]
						if(input.at(j) == ']'){
							input.replace(j,1,"");				// replaces [] bounds with test
							input.replace(i,1,"test");
							break;
						}
					}
						if(input.at(i) == '['){
							cout << "Some error with [].";
							return;
						}
					}

    }
		if(p_counter != 0){ // missing a )
			cout << "There was a parenthesis error () !\n";
			return;
		}
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
	// cout << "INPUT IS: " << endl << input << endl;
}
void print(){   // PRINTS v_lines AND v_connectors USED FOR DEBUGGING
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

      // deleting instances of ')'
      if(v_connectors.at(0)->getConnector() == ")"){
        v_connectors.erase(v_connectors.begin());


        // ERROR AFTER THIS POINT
        //logic for opening '(' : recursively call here
        else if(v_connectors.at(0)->getConnector() == "("){
          v_connectors.erase(v_connectors.begin()); //delete '('
          //count for new endindex for closing parenthesis
          int ending = 1;
          for(int j = 0; j < v_connectors.size(); j++){
            if(v_connectors.at(j)->getConnector() == "("){
              while(1){
                j++;
                if(v_connectors.at(j)->getConnector() == ")"){
                  j++;
                  break;
                }
              }
            }
            else{ ending++; }
            if(v_connectors.at(j)->getConnector() == ")"){
              j = v_connectors.size();
            }
            cout << "here" << endl;
          }
          checkexecute(ending);
        }
      }
      //ERROR BEFORE THIS POINT

      // execute arguments following ';'
      else if(v_connectors.at(0)->getConnector() == ";"){
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

      //logic for opening '(' : recursively call here


    }
  }


  return;
}

//void execute shouldnt need to look at v_connectors at all

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
    if(commandVector[0] == testchar){ // if the command is test, run test()
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
		takes in a flag:	 "", "-e", "-f", or "-d"
		also takes in path of file/directory
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
};

int main(){
    while(1){   // Keep looping
        List* shello = new List();  // Make new instance
        shello->read();             // Read in prompt and read input
        if(shello->getInputLength() == 0){} // Empty input -> re-loop
        else{                               // Input is OK
            numcon = 0;
            shello->parse();                // Parse
            shello->print();             // Print v_lines and v_connectors; (USED FOR DEBUGGING)

            vl_it = 0;
            shello->checkexecute(numcon);
						shello->execute();              // Execute
        }
        delete shello;                      // Goodbye shello :-(
    }
    return 0;
}
