#!/usr/bin/python

import sys
import urllib
import urllib2

# Check number of arguments
if len(sys.argv) != 2:
	"P: Improper arguments supplied."
	sys.exit()

# Parse data; delimiter is ','
data = sys.argv[1].split(',')
if len(data) != 3:
	print "P: Invalid data."
	sys.exit()

# Encode URL POST parameters and set up the request
url = "http://wwwtest.cse.lehigh.edu/~nad213/service.cgi"
post_data = {'temp':data[0], 'humid':data[1], 'soil':data[2]}
encoded_post_data = urllib.urlencode(post_data)
request_object = urllib2.Request(url, encoded_post_data)

# Try to access server. Handle exception if unsuccessful
try:
	response = urllib2.urlopen(request_object)
except urllib2.URLError, e:
	# Failed to connect to server
	print "P: Upload failed:", e.reason
	sys.exit()
	
# Read data from response into string
html_string = response.read()
print "P:", html_string

