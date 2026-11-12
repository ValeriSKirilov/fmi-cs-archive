#!/bin/bash

echo "Enter user:"
read user

echo "Enter filename:"
read file

last_row=$(tail -n 1 "$file")

end_index=$(expr index "$last_row" 'i')

looking_for=$(expr substr "$last_row" 1 $end_index)

total_files=$(find / -type f -name "$looking_for" -user $user 2> /dev/null | wc -l)

echo "Number of files with owner $user and name '$looking_for': $total_files" | write "$user"
