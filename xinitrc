current_datetime=$(date +"%Y-%m-%d-%H-%M")
log_file="./logs/yggwm-${current_datetime}.log"
xeyes &
# xev &
sleep 4 && kitty &
exec /home/corecaps/projects/wm/build/YggdrasilWM --log="${log_file}" --loglevel=0 >> logs/std-logs.txt 2>&1
