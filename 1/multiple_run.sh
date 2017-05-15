rm -rf res1.txt
rm -rf res2.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	./main --instance "./../instances/${array[0]}" --bestval "${array[1]}" --tmax 45 --runILS --neighbourILS 4 -- neighbourPerturb 2 --acceptanceCrit 3 --perturbFrac 0.068 --perturbRadius 0.083 --DD 0 --alpha 0.938 --T0 572 --l 128 --warmupThreshold 24 --T1 1012 >> res1.txt
done < ./1.txt

while read ligne  
do 
	IFS=',' read -a array <<<$ligne
	./main --instance "./../instances/${array[0]}" --bestval "${array[1]}" --tmax 525 --runILS --neighbourILS 4 -- neighbourPerturb 2 --acceptanceCrit 3 --perturbFrac 0.068 --perturbRadius 0.083 --DD 0 --alpha 0.938 --T0 572 --l 128 --warmupThreshold 24 --T1 1012 >> res2.txt
done < ./2.txt