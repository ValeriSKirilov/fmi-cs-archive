#!/bin/bash

if [ ! -d "$1" ]; then
	mkdir "$1"
fi

COUNT=0

for i in *; do
	if [ -f $i ] && [ -r $i ] && [ -w $i ]; then
		cp "$i" "$PWD/$1"
	elif [ -f $i ]; then
		((COUNT++))
	fi
done
echo $COUNT
