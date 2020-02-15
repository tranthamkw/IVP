#!/bin/sh

for i in $(seq 800 50 1400)
do
#background 
	sudo ./setPMT_HV "$i" 1
	sudo ./setPMT_HV 0 0
	sudo ./polarization 400 5 30 "$1" "$2"
	sudo ./setPMT_HV 80 0
#polarization at NEON setting 250V
	for j in $(seq 1 1 5)
	do
		sudo ./polarization 400 5 30 "$1" "$2"
	done

echo " done $i " | sudo mutt -s "data sub-run complete" - tranthamkw@unk.edu

done

sudo ./setVac 0 10
#pmt off
sudo ./setPMT_HV 0 0
#neon off
sudo ./setPMT_HV 0 1

