#!/bin/sh


cd /home/pi/VacuoCode/


NOW=$(date +%Y-%d-%m_%H%M)

sudo ./mainTemplate
echo "$NOW $1" | sudo mutt -s "starting auto data" -a /home/pi/MachineStatus.txt -- tranthamkw@unk.edu


for k in  $(seq 1 1 15)
do

	sudo ./setPMT_HV 850.0
	#background 
	sudo ./setBKvolts 05 1.0

	sudo ./homemotor 1 "homelog.dat"
	sudo ./polarization 400 4 30 "$1" "AutoPol$NOW.dat"
	sudo ./setBKvolts 05 1.6
#polarization 
	for j in $(seq 1 1 15)
	do
		sudo ./polarization 400 4 30 "$1" "AutoPol$NOW.dat"
	done


sudo ./mainTemplate
echo "Delay $k-$NOW $1 done " | sudo mutt -s "data sub-run complete" -a /home/pi/MachineStatus.txt -- tranthamkw@unk.edu

done

sudo ./setBKvolts 05 1.0
sudo ./setPMT_HV 0.0

