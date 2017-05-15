rm -rf res1.txt
rm -rf res2.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	./main --instance "./../instances/${array[0]}" --bestval "${array[1]}" --tmax 45 --runTabu --neighbourTabu 1 --tabuListLenght 10 --longTimeMemoryImpact 0.001 --restartThreshold 0.3 >> res1.txt
done < ./1.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	./main --instance "./../instances/${array[0]}" --bestval "${array[1]}" --tmax 525 --runTabu --neighbourTabu 1 --tabuListLenght 10 --longTimeMemoryImpact 0.001 --restartThreshold 0.3 >> res2.txt
done < ./2.txt