#!/bin/bash

echo "Enter directory:"
read dir

echo "Number of files that don't end in '.txt':"
find "$dir" -type f ! -name "*.txt" | wc -l
