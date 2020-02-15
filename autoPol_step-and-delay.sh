#!/bin/sh


cd /home/pi/VacuoCode/


NOW=$(date +%Y-%d-%m_%H%M)

sudo ./mainTemplate
echo "$NOW $1" | sudo mutt -s "starting auto data" -a /home/pi/MachineStatus.txt -- tranthamkw@unk.edu


for k in  "400"
do

sudo ./setPMT_HV 850.0

#background 
sudo ./homemotor 1 "homelog.dat"

	sudo ./setBKvolts 05 1.6
#polarization 
	for j in $(seq 1 1 15)
	do

		for i in "5" "4" "2" "1"
		do
		sudo ./polarization "$k" "$i" 20 "$1" "AutoPolDel$NOW.dat"
		done
	done


sudo ./mainTemplate
echo "Delay $k-$NOW $1 done " | sudo mutt -s "data sub-run complete" -a /home/pi/MachineStatus.txt -- tranthamkw@unk.edu

done

sudo ./setBKvolts 05 1.0
sudo ./setPMT_HV 0.0

