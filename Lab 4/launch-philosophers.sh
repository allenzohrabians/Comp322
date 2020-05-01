#!/bin/bash
n=5
programName=./dining-p
count=1
while [ ${count} -le ${n} ] ; do
  ${programName} ${n} ${count} &
  echo "PID of Philosopher ${count} is " $!
  count=$(($count + 1))
done
