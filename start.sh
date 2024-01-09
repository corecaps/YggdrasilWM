set -e

if [ ! -d "build" ]; then
    echo "Build directory not found, creating one"
    mkdir build
fi
if [ ! -d "logs" ]; then
    echo "Logs directory not found, creating one"
    mkdir logs
fi
echo $(date) > logs/std-logs.txt
echo "------------------------------------------------------------------" >> logs/std-logs.txt
cat logo.txt >> logs/std-logs.txt
cat logo.txt
if [ ! -f "build/CMakeCache.txt" ]; then
    echo "CMakeCache.txt not found, running cmake"
    cd build
    cmake ..
    cd ..
fi
cd build
echo "Running make"
make
cd ..
xinit ./xinitrc -- /usr/bin/Xephyr :100 -ac -screen 800x600 -host-cursor 
