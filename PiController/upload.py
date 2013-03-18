#!/usr/bin/python

import sys

if len(sys.argv) == 2:
	# Parse data; delimiter is ','
	data = sys.argv[1].split(',')
	if len(data) == 3:
		print "P: Temp =", data[0]
		print "P: Humid =", data[1]
		print "P: Soil =", data[2]
	else:
		print "P: Invalid data."
else:
	print "P: Improper arguments supplied."

