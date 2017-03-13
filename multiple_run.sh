rm -rf res.txt

echo "instance:rndtr:rndtrPPD:rndex:rndexPPD:rndin:rndinPPD:rztr:rztrPPD:rzex:rzexPPD:rzin:rzinPPD" >> res.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	./code/main "./instances/${array[0]}" "${array[1]}" >> res.txt
done < ./instances/bestSolutions.txt