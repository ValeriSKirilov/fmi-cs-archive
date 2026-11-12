#!/bin/bash

echo "Enter file 1:"
read file1

echo "Enter file 2:"
read file2

echo "Enter file 3:"
read file3

echo "Name lengths:"
echo "$file1 - $(expr length "$file1")"
echo "$file2 - $(expr length "$file2")"
echo "$file3 - $(expr length "$file3")"

echo "Sizes of the files:"
wc -c "$file1" | awk '{print $2 " - " $1 " bytes"}'
wc -c "$file2" | awk '{print $2 " - " $1 " bytes"}'
wc -c "$file3" | awk '{print $2 " - " $1 " bytes"}'
