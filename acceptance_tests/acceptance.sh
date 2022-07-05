#!/bin/bash
# the & runs a command/program in the background
./Webserver.out &

# Save the PID to kill the webserv
PID=$!

# sleep for 1 second to give the server time to start-up
sleep 1

# save the return val of the tests for actions
ExitCode=0

# run the tests
cd acceptance_tests/PyTests/
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

rm -rf ./root/uploads/*.txt

sleep 0.5

kill $PID
exit $ExitCode