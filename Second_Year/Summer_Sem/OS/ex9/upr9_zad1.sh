#!/bin/bash

if [ $# -eq 2 ]; then
	if [ $(find "$1" -type d 2> /dev/null) ]; then 
		find "$1" -type f -size +$2c
	else
		echo "The specified directory doesn't exist!"
	fi
else
	echo "Invalid ammount of parameters!"
fi	
