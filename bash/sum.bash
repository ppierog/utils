#!/bin/bash


start=`date +%s`
s=0
for (( c=1; c<=100000000; c++ ))
do 
    s=$((s+c))
done

end=`date +%s`
echo $s
echo Elapsed $(expr $end - $start) seconds
