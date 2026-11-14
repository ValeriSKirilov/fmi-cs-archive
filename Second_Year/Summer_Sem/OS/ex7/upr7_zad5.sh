#!/bin/bash

echo "Enter the first user:"
read user1

echo "Enter the second user:"
read user2

who | grep -vwE "$user1|$user2"
