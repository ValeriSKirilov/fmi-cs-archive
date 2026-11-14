#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "Invalid amount of parameters!"
fi

COUNT1=$(find "$1" -type f -name "*.out" | wc -l)
COUNT2=$(find "$2" -type f -name "*.out" | wc -l)

if [ $COUNT1 -gt $COUNT2 ]; then
	read NEW_DIR
	mkdir $NEW_DIR
	for file in "$1"/*; do
		if [[ -f "$file" && -r "$file" && -w "$file" && ! -x "$file" ]]; then
			mv "$file" $NEW_DIR
		fi
	done
fi
