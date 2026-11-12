#!/bin/bash

FILE=$1

read MESSAGE

shift

for name in "$@"; do
	if who | grep -wq "$name"; then
		printf "$MESSAGE\n\n$(cat $FILE)\n" | write $name
	fi	
done
