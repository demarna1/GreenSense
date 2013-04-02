#!/bin/bash

if [[ -z $1 ]]
then 
	echo "Error: no data supplied as parameter."
	exit 1
fi

# Write data out to a file called "measure"
echo $1 > measure
scp measure nad213@sunlab.cse.lehigh.edu:WWW-data/measure | \
	cat > /dev/null
rm -f measure

# Also execute PHP > html script
ssh nad213@sunlab.cse.lehigh.edu << DONE > /dev/null
	cat WWW-data/measure >> WWW-data/data.txt	
	rm -f WWW-data/measure
	WWW-data/service.php > WWW-data/index.html
DONE

