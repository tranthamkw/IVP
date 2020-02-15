#!/bin/sh

cd /home/pi/VacuoCode/

sudo ./mainTemplate
echo "$NOW $1" | sudo mutt -s "starting auto data" -a /home/pi/MachineStatus.txt -- tranthamkw@unk.edu

sudo ./homemotor 1

NOW=$(date +%Y-%d-%m_%H%M)
sudo ./setPMT_HV 850.0

for i in $(seq 0 2 10)
do

#background 
	sudo ./setBKvolts 05 1.0
	sudo ./polarization 400 5 30 "$1" "AutoStepTest$i-$NOW.dat"
#  ./polarization <total num steps> <step size> <delaytime between data points mS> <comments> <autoSummary file>

	sudo ./setBKvolts 05 1.6
	sudo ./stepmotor 1 "$i" 1
# intentially step motor by $i steps

#polarization  starting from this position
	for j in $(seq 1 1 5)
	do
	sudo ./polarization 400 5 30 "$1" "AutoStepTest$i-$NOW.dat"
	done
	sudo ./homemotor 1 "motorlog.dat"
# expect to see something different than 100

#background 
	sudo ./setBKvolts 05 1.0
	sudo ./polarization 400 5 30 "$1" "AutoStepTest$i-$NOW.dat"
	sudo ./setBKvolts 05 1.6
# polarization
	for j in $(seq 1 1 5)
	do
	sudo ./polarization 400 5 30 "$1" "AutoStepTest$i-$NOW.dat"
	done

	sudo ./homemotor 1 "motorlog.dat"
#expect to see 100


done

sudo ./mainTemplate
echo "StepTest $i-$NOW $1 done " | sudo mutt -s "data sub-run complete" -a /home/pi/MachineStatus.txt -- tranthamkw@unk.edu


sudo ./setBKvolts 05 1.0
sudo ./setPMT_HV 0.0

