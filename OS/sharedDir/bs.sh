#!/bin/bash

counter=0

while [ $counter -le 5 ]; do
	echo "Hi" | write $1
	(( counter++ ))
done
