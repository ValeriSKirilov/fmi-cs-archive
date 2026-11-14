#!/bin/bash

while ! who | grep -wq "$1"; do
	sleep 30
done

echo "User $1 is now online!"
