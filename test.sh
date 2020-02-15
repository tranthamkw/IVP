#!/bin/sh

NOW=$(date +%Y-%d-%m)
STR="/home/pi/VacuoCode/"


for i in "650" "350" "150" "30" "3"
do
echo $STR
echo "Filename$i-$NOW.dat"


done

