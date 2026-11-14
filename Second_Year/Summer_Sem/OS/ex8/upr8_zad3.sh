#!/bin/bash

for i in 1 2 3; do
	cp $1 "$1_$i"
done

grep "$2" $1

grep "$2" $1 | wc -l > $3

#mv $1 /info/$1
