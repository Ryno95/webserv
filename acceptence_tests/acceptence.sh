#!/bin/bash
# the & run a command/program in the background
./../Webserver.out &
# Get its PID
PID=$!
sleep 2
python3 TestClient.py
T1=$?
# printf "Return number is %s\n" 
sleep 2
kill $PID
exit $T1