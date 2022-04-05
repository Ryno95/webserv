#!/bin/bash
# the & runs a command/program in the background
./Webserver.out &

# Save the PID to kill the webserv
PID=$!
# sleep for 1 second to give the server time to start-up
sleep 2
# run the tests

# curl localhost:4242
python3 acceptence_tests/TestClient.py 
# # save the return val of the tests foor actions
T1=$?

kill $PID
exit $T1