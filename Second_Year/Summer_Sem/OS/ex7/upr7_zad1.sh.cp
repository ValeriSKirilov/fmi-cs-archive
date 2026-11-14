#!/bin/bash

echo "Enter number one: "
read num1

echo "Enter number two: "
read num2

sum=$((num1 + num2))
prod=$((num1 * num2))

echo "The sum of $num1 and $num2 is: $sum"
echo "The product of $num1 and $num2 is: $prod"
