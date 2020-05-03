#!/bin/bash
n=5
programName=./dining-p
count=1
while [ ${count} -le ${n} ] ; do
  ${programName} ${n} ${count} &
  echo "PID of Philosopher ${count} is " $!
  count=$(($count + 1))
done
error_exit()
{
  echo "${programName}: ${1:-"Unknown Error"}" 1>&2
  exit 1
}
