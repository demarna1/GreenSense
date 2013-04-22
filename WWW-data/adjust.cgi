#!/usr/bin/python
import cgi

def main():
    print "Content-type: text/html\n"
    form = cgi.FieldStorage() #Parse query
    
    if form.has_key("templow") and form.has_key("temphigh")\
       and form.has_key("humidlow") and form.has_key("humidhigh")\
       and form.has_key("soillow") and form.has_key("soilhigh")\
       and form.has_key("fanon"):
       
       # Get values
       templow = form["templow"].value
       temphigh = form["temphigh"].value
       humidlow = form["humidlow"].value
       humidhigh = form["humidhigh"].value
       soillow = form["soillow"].value
       soilhigh = form["soilhigh"].value
       fanon = form["fanon"].value

       # Write to settings file
       f = open('settings.txt', 'w')
       f.write(templow + "\n" + temphigh + "\n")
       f.write(humidlow + "\n" + humidhigh + "\n")
       f.write(soillow + "\n" + soilhigh + "\n")
       f.write(fanon + "\n")
       f.close()

       print "Got here"

       # Launch TCP client to send data to RPi
       
    else:
	print "<h1>Error: No data</h1>"

main()
