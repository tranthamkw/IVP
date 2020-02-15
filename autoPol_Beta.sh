#!/bin/sh


for k in  $(seq 340 1 370)

do

NOW=$(date +%Y-%m-%d_%H%M)
sudo ./setPMT_HV 1000 1
sudo ./homemotor 1 "homelog.dat"
sudo ./stepmotor 1 "$k" 1

#polarization 
	for j in $(seq 1 1 5)
	do
		sudo ./polarization 400 1 30 "steps=$k" "AutoBeta$k-$NOW.dat"
	done


done



