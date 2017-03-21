rm -rf res.txt

echo "rndtr:rndtrPPD:rndex:rndexPPD:rndin:rndinPPD:rztr:rztrPPD:rzex:rzexPPD:rzin:rzinPPD:rndVND1:rndVND1PPD:rzVND1:rzVND1PPD:rndVND2:rndVND2PPD:rzVND2:rzVND2PPD" >> res.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	./code/main "./instances/${array[0]}" "${array[1]}" >> res.txt
done < ./instances/bestSolutions.txt