#!/bin/bash

if [[ -z $1 ]]
then 
	echo "Error: no data supplied as parameter."
	exit 1
fi

echo $1 > measure
scp measure nad213@sunlab.cse.lehigh.edu:WWW-data/measure
rm -f measure

# Also execute PHP > html script
ssh -l nad213 sunlab.cse.lehigh.edu << DONE
	cat WWW-data/measure >> WWW-data/data.txt	
	rm -f WWW-data/measure
DONE

