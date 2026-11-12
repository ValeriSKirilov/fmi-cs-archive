#!/bin/bash

temp_file="temp_file.txt"

mv "$1" "$temp_file"
mv "$2" "$1"
mv "$temp_file" "$2"
