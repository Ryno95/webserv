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
python3 GETTests.py
ExitCode+=$?

python3 POSTTests.py
ExitCode+=$?

pwd
python3 CGITests.py
ExitCode+=$?

rm -rf ./root/uploads/*.txt

sleep 1
kill $PID
exit $ExitCode