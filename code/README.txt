the code is compiled with the command "make" in the code directory

all .o file can be deleted with "make clean" in the code directory

the number of iteration for the average and value to measure must be chose with the preprocessor variable at the begenning of "main.cpp"

to run the code the command is
	./main --instance path_to_the_intance --bestval optimal_value_of_this_instance --tmax time_budget

then to permormed a tabu search you must add
	--runTabu --neighbourTabu neighbour_code --tabuListLenght tabu_tenure --longTimeMemoryImpact long_time_memory_factor --restartThreshold restart_ratio

and to use ILS (both can be use in the same run, one after the other)
	--runILS --neighbourILS local_search_code -- neighbourPerturb neighbour_code --acceptanceCrit criterion_code --perturbFrac perturbation_ration --perturbRadius radius_ratio --DD deepest_descent_boolean
	and if you use the metropolis criterion (code 3) you shall add
		--alpha cooling_factor --T0 initial_temperature --l cooling_speed --warmupThreshold warmup_limit --T1 temperature_after_warmup
	and if you use the metropolis like criterion (code 4) you shall add
		--lambda temperature_factor