#!/bin/bash
# the & runs a command/program in the background
./Webserver.out  | cat -e &

# Save the PID to kill the webserv
PID=$!

# sleep for 1 second to give the server time to start-up
sleep 2

# run the tests
python3 acceptance_tests/TestClient.py

# save the return val of the tests for actions
T1=$?

kill $PID
exit $T1