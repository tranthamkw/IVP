#!/bin/sh

cd /home/pi/VacuoCode/

#sudo ./mainTemplate
echo "$NOW $1" | sudo mutt -s "starting auto data" -a /home/pi/MachineStatus.txt -- tranthamkw@unk.edu


for k in  $(seq 1 1 8)

#for k in "0" "50" "200" "650"
do

NOW=$(date +%Y-%m-%d_%H%M)
sudo ./setPMT_HV 1000
sudo ./homemotor 1 "homelog.dat"
sudo ./stepmotor 1 357 1
for i in $(seq 1 1 5)
do
	sudo ./setVac 0 10
#background at atmo
#	sudo ./setBKvolts 192 1.0
	sudo ./setUSB_Aout 0 0
	sudo ./polarization 400 1 30 "$1" "AutoPolVac$k-$NOW.dat"
#	sudo ./setBKvolts 192 4.0
	sudo ./setUSB_Aout 0 760

#polarization at atmo
	for j in $(seq 1 1 5)
	do
		sudo ./polarization 400 1 30 "$1" "AutoPolVac$k-$NOW.dat"
	done

	sudo ./setVac 1 10
#background at new pressure
#	sudo ./setBKvolts 192 1.0
	sudo ./setUSB_Aout 0 0
	sudo ./polarization 400 1 30 "$1" "AutoPolVac$k-$NOW.dat"
#	sudo ./setBKvolts 192 4.0
	sudo ./setUSB_Aout 0 760

# polarization at new pressure
	for j in $(seq 1 1 5)
	do
		sudo ./polarization 400 1 30 "$1" "AutoPolVac$k-$NOW.dat"
	done

done

#sudo ./mainTemplate
echo "Pressure $k-$NOW $1 done " | sudo mutt -s "data sub-run complete" tranthamkw@unk.edu

done


sudo ./setVac 0 10
sudo ./setPMT_HV 0
#sudo ./setBKvolts 192 1.0
sudo ./setUSB_Aout 0 0



