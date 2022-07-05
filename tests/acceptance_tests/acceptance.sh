#!/bin/bash

# Compile and move executable to correct directory
make -C src/ -j5
mv src/webserv server/

# The & runs a command/program in the background
cd server/
./webserv &
cd ..

# Save the PID to kill the webserv
PID=$!

# sleep for 1 second to give the server time to start-up
sleep 1

# save the return val of the tests for actions
ExitCode=0

# run the tests

cd tests/acceptance_tests/PyTests/
for i in {1..10}
do
    python3 GETTests.py
    ExitCode+=$?
done

for i in {1..3}
do
    python3 POSTTests.py
    ExitCode+=$?
done

for i in {1..3}
do
    python3 DELETETests.py
    ExitCode+=$?
done

for i in {1..10}
do
    python3 CGITests.py
    ExitCode+=$?
done

sleep 0.5

kill $PID
exit $ExitCode
