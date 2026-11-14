#!/bin/bash

echo "Enter user:"
read user

total_size=$(du -sh /home/KN/ 2> /dev/null | awk '{print $1}')

echo "$total_size" | write "$user"
