#!/bin/bash

LAST_ROW=$(tail -n1 "$2")
IDX=$(expr index "$LAST_ROW" "1")
LOOKING_FOR=${LAST_ROW:0:$((IDX - 1))}

FOUND_FILES=$(find $HOME/.. -type f -name "$LOOKING_FOR" | wc -l)

if [ $FOUND_FILES -ne 0 ]; then
	echo $FOUND_FILES
else
	echo "No files with name $LOOKING_FOR found" | write $1
fi
