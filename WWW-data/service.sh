#!/usr/bin/bash
cd /home/nad213/WWW-data
cat measure >> data.txt
rm -f measure
php index.php > index.html
