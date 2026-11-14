#!/bin/bash

COUNT=1
for i in "$@"; do
	echo "Argument $COUNT: $i"
	(( COUNT++ ))
done
echo
echo "Arguments count: $#"

FILES=$(find ~ -type f)
set $FILES

shift 2

COUNTER=1
for i in "$@"; do
        echo "Argument $COUNTER: $i"
        (( COUNTER++ ))
done
echo
echo "Arguments count: $#"
