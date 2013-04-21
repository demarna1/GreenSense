#!/bin/bash

while true; do
	sleep 10
	curl -s www.cse.lehigh.edu/~nad213/settings.txt | cat
done

