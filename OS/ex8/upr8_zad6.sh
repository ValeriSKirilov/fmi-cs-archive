#!/bin/bash

TOTAL_SIZE=$(find "$1" -type f -exec wc -c {} + | tail -n 1 | awk '{print $1}')

if who | grep -wq "$2"; then
	echo "The total size of all the files in $1 is $TOTAL_SIZE bytes" | write "$2"
else
	echo "User $2 is not online"
fi
