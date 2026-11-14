#!/bin/bash

COUNT1=$(find $1 -type f -name "*.c" | wc -l)
COUNT2=$(find $2 -type f -name "*.c" | wc -l)

COUNT=$(($COUNT1 + $COUNT2))

if [ $COUNT -gt 20 ]; then
	touch $PWD/sources.txt
	find $1 -type f -name "*.c" > sources.txt
	find $2 -type f -name "*.c" >> sources.txt
	chmod o=+r sources.txt
else
	echo "Total count of c files: $COUNT"
fi
