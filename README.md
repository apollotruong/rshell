# rshell
Patrick Aben, Apollo Truong

# Overview:

 A clone of bash made in C++. Able to handle multiple commands, and comments through one line. Able to handle basic linux commands such as `ls`, `cd` , `mkdir`, `rm`, etc. Until the user types `exit` to exit the program. Multiple commands will be handled by the `;` connector, the logic for `||` (OR) and `&&` (AND) is added as well. `||` is handled by connecting two lines together, and the next line of code will be either run or not run depending on the first command (TRUE||FALSE) or (FALSE||TRUE) . `&&` Will be handled by having the first command run, the connecting command will also run. Anything after a `#` (comment) will be ignored. Precedent operators are also be included if you would like to use them `(` and `)`.This program will also run the test command, but will **ALSO** echo to the terminal if the path of a file or directory exists, `(TRUE)`, or if it does not exist `(FALSE)`. *Wow!*

# Some Examples

Example #1
```
$ echo A && echo B || echo C && echo D
A
B
D
```
Example #2
```
$ echo A; echo B || echo C;
A
B
```
Example #3
```
$ ( echo A && echo B ) || ( echo C && echo D )
A
B
```

# Running Instructions

`git clone https://github.com/apollotruong/rshell.git`

`cd rshell`

`git checkout hw3`

`make`

`bin/rshell`

# Known Bugs

- *hold up!*