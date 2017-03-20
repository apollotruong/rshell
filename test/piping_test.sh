cat *.txt | sort | uniq > result-file
cat *.txt | sort | uniq > result-file # comment
cat *.txt | sort | uniq > result-file # | echo fail
cat t1.txt | input.txt | ls > output-file
cat t1.txt | input.txt | ls > output-file #comment
cat t1.txt | input.txt | ls > output-file # | echo fail
cat < result-file | tr A-Z a-z | tee newout1 | tr a-z A-Z > newout2
cat < result-file | tr A-Z a-z | tee newout1 | tr a-z A-Z > newout2 # comment
