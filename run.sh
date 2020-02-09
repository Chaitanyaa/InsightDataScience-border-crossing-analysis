#!/bin/bash
#
# Use this shell script to compile (if necessary) your code and then execute it. Below is an example of what might be found in this file if your program was written in Python
#
#python ./src/purchase_analytics.py ./input/order_products.csv ./input/products.csv ./output/report.csv

#!/bin/sh

echo "Now we are going to compile the original c++ file..."

g++ -std=c++11 ./src/border_cross.cpp -o Program

if [ $? -ne 0 ]
then
	echo "Compillation failed."
else
	echo "Compillation successfully completed."
	echo "Start the program..."
	time=$(date +%s)
	##ls | xargs -n1 --max-procs=4 ./Program
	./Program
	if [ $? -ne 0 ] 
	then 
		echo "Time of work is " $(($(date +%s)-$time)) " second(s)"
		echo "Program works incorrectly."
		while true; do
		    read -p "Do you want to restart the script?" yn
		    case $yn in
			[Yy]* ) ./run.sh; break;;
			[Nn]* ) exit;;
			* ) echo "Please answer yes or no.";;
		    esac
		done
	else
		echo "Time of work is " $(($(date +%s)-$time)) " second(s)"
		echo "Program works correctly."	
	fi
	echo "Results are stored in the following repo: "output""
fi
