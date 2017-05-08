rm -rf res.txt

echo "instance:tabuE:tabuI" >> res.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	echo -n "$array:" >> res.txt
	./code/main --instance "./instances/${array[0]}" --bestval "${array[1]}" >> res.txt
done < ./instances/bestSolutions.txt