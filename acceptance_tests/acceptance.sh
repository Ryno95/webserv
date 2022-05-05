#!/bin/bash
# the & runs a command/program in the background
./Webserver.out &

# Save the PID to kill the webserv
PID=$!

# sleep for 1 second to give the server time to start-up
sleep 2

# save the return val of the tests for actions
ExitCode=0

# run the tests
python3 acceptance_tests/PyTests/GETTests.py
ExitCode+=$?

python3 acceptance_tests/PyTests/POSTTests.py
ExitCode+=$?


kill $PID
exit $ExitCode