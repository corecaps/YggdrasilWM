current_datetime=$(date +"%Y-%m-%d-%H-%M")
log_file="./logs/yggwm-${current_datetime}.log"
xeyes &

xev &
#sleep 3 && killall -SIGUSR1 Xephyr

sleep 4 && xclock &
sleep 6 && kitty &
exec valgrind --leak-check=full --track-origins=yes /home/corecaps/projects/wm/build/YggdrasilWM --log="${log_file}" --loglevel=0 >> logs/std-logs.txt 2>&1
