# class Rshell
```
protected:
vector<vector<Parameter*>>   v_lines;
vector<Connectors*>  v_connectors;
string  input;

public:
Rshell(){}                      // Default Constructor

void    read();                 // Loads input with cin, also exits if input is exit
void    parse();                // Organizes input into v_lines & v_connectors
void    execute();              // Execute v_lines according to v_connectors
                                // Does logic for v_connectors
```

# class Parameter : public Rshell
```
protected:
bool    usage = false;
string  parameter;

public:
Parameter(string s): usage(false), parameter(s){}

string  getParameter(); //
bool    getUsed();      // Returns usage
void    trueUsed();     // Change usage = true
void    falseUsed();    // Change usage = false
```

# class Connector : public Rshell
```
protected:
string  connector;

public:
Connector(string s): valid(false), connector(s){}

string  getConnector(); // returns connector;
```

# Notes:

Code now finished.
Slight changes to implementation has made the source code slightly deviate from the skeleton shown above. Changes do not deviate from the class system, only implementation/declaration of some protected variables and public functions.

Known bugs: Special cases using comments gives unexpected/unwanted results. Output a comment line with an output declaring the absence of an executable. Comments in the middle of code are ignored, and not output.

Program can only exit if "exit" command is called in a single line with no other arguments. Cannot be run at the end of other arguments.

Exit function works when user calls it, but a scipt (exit.sh) with exit as its only code for some reason will not exit out of file.
