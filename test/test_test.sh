test -d folder
test -e file
test -f file
[-d folder]
[-e file]
[-f file]
test -f file
[-f file]
test -e file && test -f file
[test -e file] || [test -d file]
[test -d file] && test -d folder
