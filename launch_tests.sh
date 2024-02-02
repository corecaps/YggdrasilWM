#!/bin/bash
#Build if needed 
cd build
make

# Wait for 4 seconds to allow Xephyr to start
clear
figlet YggdrasilWM
figlet :Unit Tests:
# Launch YggdrasilWM_tests
valgrind --leak-check=full --track-origins=yes ./YggdrasilWM_tests
TESTS_RESULT=$?

#return the test result
exit $TESTS_RESULT

