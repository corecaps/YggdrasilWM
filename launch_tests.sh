#!/bin/bash
#Build if needed 
cd build
make
# Launch Xephyr in the background
/usr/bin/Xephyr :1 -ac -screen 800x600 -host-cursor > /dev/null 2>&1 &
XEPHYR_PID=$!

# Wait for 4 seconds to allow Xephyr to start
sleep 4
clear
figlet YggdrasilWM
figlet :Unit Tests:
# Launch YggdrasilWM_tests
valgrind --leak-check=full --track-origins=yes ./YggdrasilWM_tests
TESTS_RESULT=$?

# Kill the Xephyr process
kill $XEPHYR_PID
#return the test result
exit $TESTS_RESULT

