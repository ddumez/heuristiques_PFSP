rm -rf res.txt

echo "instance:tabuE2" >> res.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	echo -n "$array:" >> res.txt
	./main --instance "./../instances/${array[0]}" --bestval "${array[1]}" --tmax 45 --runTabu --neighbourTabu 1 --tabuListLenght 9 --longTimeMemoryImpact 0.26 --restartThreshold 0.048 >> res.txt
done < ./1.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	echo -n "$array:" >> res.txt
	./main --instance "./../instances/${array[0]}" --bestval "${array[1]}" --tmax 525 --runTabu --neighbourTabu 1 --tabuListLenght 9 --longTimeMemoryImpact 0.26 --restartThreshold 0.048 >> res.txt
done < ./2.txt