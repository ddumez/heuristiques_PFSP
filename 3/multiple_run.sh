rm -rf res.txt

echo "instance:2.0:1.0:0.5:0.1" >> res.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	echo -n "$array:" >> res.txt
	./main --instance "./../instances/${array[0]}" --bestval "${array[1]}" --tmax 450 --runTabu --neighbourTabu 1 --tabuListLenght 10 --longTimeMemoryImpact 0.001 --restartThreshold 0.3 >> res.txt
done < ./1.txt