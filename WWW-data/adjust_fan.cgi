#!/usr/bin/python
import cgi

def main():
    print "Content-type: text/html\n"
    form = cgi.FieldStorage() #Parse query
    
    if form.has_key("fan"):
       
       # Get values
       fan = form["fan"].value

       # Write to settings file
       f = open('settings.txt', 'w')
       f.write("70\n90\n35\n75\n2\n4\n")
       f.write(fan + "\n")
       f.close()

       print "<h1>Your settings have been saved</h1>"

       print "<form name=\"input\" action=\"http://www.cse.lehigh.edu/~nad213\" method=\"get\">"
       print "<input type=\"submit\" value=\"Return to Webpage\">"
       print "</form>"
       
    else:
	print "<h1>Error: No data</h1>"

main()
