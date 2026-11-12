#!/bin/bash

echo "Enter text of 4 word:"
read text

word_count=$(echo "$text" | wc -w)

if [ "$word_count" -eq 4 ]; then
	echo $(expr index "$text" a)
	echo $(expr substr "$text" 5 10)
else
	echo "Invalid imput!"
fi
