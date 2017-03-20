cat < input.txt
cat < t1.txt
cat < input.txt #comment
cat < input.txt #|echo fail
cat < t1.txt | tr A-Z a-z | tee newout1 | tr a-z A-Z > newout2
cat < t1.txt | tr A-Z a-z | tee newout1 | tr a-z A-Z > newout2 # ; echo fail
