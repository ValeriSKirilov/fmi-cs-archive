#!/bin/bash

read LOOKING_FOR

for i in "$@"; do
	echo "Number of rows in $1 containing the wanted string: $(grep "$LOOKING_FOR" "$i" | wc -l)"
	echo
done

