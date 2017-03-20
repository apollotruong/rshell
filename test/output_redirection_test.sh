input.txt > newout3.txt
t1.txt > newout3.txt
input.txt >> newout3.txt
echo a > newout4.txt
echo b >> newout4.txt
echo a > newout5.txt # comment
echo b >> newout5.txt # | echo fail
cat < newout4.txt | tr A-Z a-z | tee newout1 | tr a-z A-Z > newout2
cat < newout4.txt | tr A-Z a-z | tee newout1 | tr a-z A-Z > newout2 # comment
