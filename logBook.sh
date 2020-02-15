#!/bin/sh



./autoDataFolder

NOW=$(date +%Y-%m-%d_%H%M%S)
FOLDER=$(date +%Y-%m-%d)

nano "/home/pi/Data/$FOLDER/LogBook$NOW.txt"








