#!/bin/sh

sudo ./setPMT_HV 1000 1

for i in $(seq 1.0 0.1 4.0)
do
sudo ./homemotor 1 "homelog.dat"
sudo ./stepmotor 1 361 1
	sudo ./setBKvolts 05 "$i"
#polarization at LED setting $i
	for j in $(seq 1 1 10)
	do
		sudo ./polarization 400 1 30 "$1" "$2"
	done



echo " done $i " | sudo mutt -s "data sub-run complete" - tranthamkw@unk.edu

done

sudo ./setBKvolts 05 1.0
sudo ./setPMT_HV 0 1
