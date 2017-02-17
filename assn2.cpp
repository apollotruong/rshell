#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm>

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

class Rshell;                   // Pre-definition of Rshell
class List;
class Parameter;                // Pre-definition of Parameter
class Connector;                // Pre-definition of Connector

class Rshell{
public:
Rshell(){}
// virtual void read();
// virtual void parse();
// virtual void execute();
// virtual int getInputLength();
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
vector<Parameter*> v_lines;                // Vector of Commands
vector<Connector*> v_connectors;           // Vector of connectors
int lines_counter;                      // counts how many lines in v_lines
string  input;                          // Whole input, (includes connectors)

public:
List(): lines_counter(0), input(""){}
void read(){                         // Loads input with cin, also exits if input is exit
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

    for(unsigned i = 0; i < input.size(); i++){                 // Handle ';'
        if(input.at(i) == ';'){
            v_connectors.push_back(new Connector(";"));
        }
        else if(input.at(i) == '&'){
            if(input.at(i+1) == '&'){
                v_connectors.push_back(new Connector("&&"));    // Handle "&&"
            }
        }
        else if(input.at(i) == '|'){
            if(input.at(i+1) == '|'){
                v_connectors.push_back(new Connector("||"));    // Handle "||"
            }
        }
        else if(input.at(i) == '#'){                // If its a comment, do nothing
            if(input.at(i+1) == '#'){
        	v_connectors.push_back(new Connector("#"));
            }
        }
    }
    char* input_c = (char*)input.c_str();
    char* tok = strtok(input_c, ";|&#");

    while(tok != NULL){
        string temp = tok;
        whitespace = 0;                 // Removing whitespace
        while (temp.at(whitespace) == ' '){
            whitespace++;
        }
        temp = temp.substr(whitespace);
        v_lines.push_back(new Parameter(temp));
        tok = strtok(NULL, ";|&#");
    }
}
void print(){   // PRINTS v_lines AND v_connectors USED FOR DEBUGGING
    cout << "v_lines:" << endl;
    for(unsigned i = 0; i < v_lines.size(); i++){
        cout << "index " << i << ": " << v_lines.at(i)->getParameter() << endl;
    }
    cout << endl << "v_connectors:" << endl;
    for(unsigned i = 0; i < v_connectors.size(); i++){
        cout << "index " << i << ": " << v_connectors.at(i)->getConnector() << endl;
    }
}
// Execute v_lines according to v_connectors
// Does logic for v_connectors
// forking and execvp will be handled here

void execute(){
	int i = 0; // lines iterator
	if(v_connectors.size() == 0){ //this means
		// checks if there are any connectors, if not, execute and return
		stringstream ss(v_lines.at(0)->getParameter());
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

		pid_t pid;
		int status;
		if((pid = fork()) < 0){
			cout << "Error: failed to fork child process" << endl;
			exit(1);
		}
		else if(pid == 0){
			v_lines.at(i)->trueUsed();
			if(execvp(command[0], command) < 0){
				cout << "Error: nothing to execute" <<endl;
				exit(1);
			}
		}
		else{
			while(wait(&status) != pid); // wait for parent
		}

		return;
	}
	else{
		// if there are connectors, execute vline 0 and move on
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

		pid_t pid;
		int status;
		if((pid = fork()) < 0){
			cout << "Error: failed to fork child process" << endl;
			exit(1);
		}
		else if(pid == 0){
			v_lines.at(i)->trueUsed();
			if(execvp(command[0], command) < 0){
				cout << "Error: failed to execute" <<endl;
				exit(1);
			}
		}
		else{
			while(wait(&status) != pid); // wait for parent
		}
		v_lines.at(i)->trueUsed();
		i++;
	}
	while(v_connectors.size() > 0 ){ // always delete the connector after usingit
		if(v_connectors.at(0)->getConnector() == ";"){
      //always execute lines with ';', and then increment v_lines
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


      v_connectors.erase(v_connectors.begin());
      v_lines.at(i)->trueUsed();
      i++;
		}
		else if(v_connectors.at(0)->getConnector() == "||"){
			// execute if Usage of last parameter is false
			if(!v_lines.at(i-1)->getUsed()){
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
			v_lines.at(i)->trueUsed();
    }
    else v_lines.at(i)->falseUsed();
			i++;
			v_connectors.erase(v_connectors.begin());
		}
		else if(v_connectors.at(0)->getConnector() == "&&"){
			//execute if Usage of last parameter is true;


			if(v_lines.at(i-1)->getUsed() == true){
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

			pid_t pid;
			int status;
			if((pid = fork()) < 0){
				cout << "Error: failed to fork child process" << endl;
				exit(1);
			}
			else if(pid == 0){
				v_lines.at(i)->trueUsed();
				if(execvp(command[0], command) < 0){
					cout << "Error: failed to execute" <<endl;
					exit(1);
				}
			}
			else{
				while(wait(&status) != pid); // wait for parent
			}
			v_lines.at(i)->trueUsed();
			i++;
			v_connectors.erase(v_connectors.begin());
			}
		}
		else if(v_connectors.at(0)->getConnector() == "#"){
			// never execute, simply cout, and move on
      cout << "#" << v_lines.at(i)->getParameter() << endl;
			i++;
			v_connectors.erase(v_connectors.begin());
		}
	}
	return;
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
            shello->parse();                // Parse
            // shello->print();             // Print v_lines and v_connectors; (USED FOR DEBUGGING)
            shello->execute();              // Execute
        }
        delete shello;                      // Goodbye shello :-(
    }
    return 0;
}
