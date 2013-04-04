#!/bin/bash

if [[ -z $1 ]]
then 
	echo "Error: no data supplied as parameter."
	exit 1
fi

# Write data out to a file called "measure"
echo $(date +%s)","$1 > measure
scp measure nad213@sunlab.cse.lehigh.edu:WWW-data/measure
rm -f measure

# Execute script on server
ssh nad213@sunlab.cse.lehigh.edu WWW-data/service.sh

