#!/bin/bash

((count=0))

while :
do
	count=$(echo $count+0.16 | bc -l)
	echo W 0.2 $count 0.8 0.2
done
