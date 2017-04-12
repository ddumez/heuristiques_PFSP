rm -rf res.txt

echo "instance:rndtr:rndtrDD:rndex:rndexDD:rndin:rndinDD:rztr:rztrDD:rzex:rzexDD:rzin:rzinDD:rndVND1:rndVND1DD:rzVND1:rzVND1DD:rndVND2:rndVND2DD:rzVND2:rzVND2DD" >> res.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	echo -n "$array:" >> res.txt
	./code/main "./instances/${array[0]}" "${array[1]}" >> res.txt
done < ./instances/bestSolutions.txt