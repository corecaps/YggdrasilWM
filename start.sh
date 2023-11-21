set -e
echo $(date) > logs/std-logs.txt
echo "------------------------------------------------------------------" >> logs/std-logs.txt
cd build
cmake ..
cmake --build .
cd ..
xinit ./xinitrc -- /usr/bin/Xephyr :100 -ac -screen 800x600 -host-cursor 
