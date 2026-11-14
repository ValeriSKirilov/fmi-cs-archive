#!/bin/bash

echo "Enter file name:"
read file

echo "File contents:"
cat "$file"

cp "$file" "$file.cp"
echo "File copied succesfully"

echo "Size of the file's name:"
echo ${#file}

echo "Size in bites:"
wc -c "$file"
