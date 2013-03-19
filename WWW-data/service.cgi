#!/usr/bin/python
import cgi

def main():
    print "Content-type: text/html\n"
    form = cgi.FieldStorage() # Parse query
    if form.has_key("temp") and form.has_key("humid")\
        and form.has_key("soil"):
	temp = form["temp"].value
	humid = form["humid"].value
	soil = form["soil"].value
	
	# Append to file
	f = open('data.txt', 'a')
	f.write(temp + "," + humid + "," + soil + "\n") 
	f.close()        
	print temp
	print humid
	print soil
    else:
	print "<h1>Error: No data</h1>"

main()