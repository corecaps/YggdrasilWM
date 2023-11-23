# YggdrasilWM 
**Version 0.1.1**

## What is YggdrasilWM?
YggdrasilWM is a tiling window manager for X11. It is written in C++ and uses the Xlib library. It is inspired by dwm and xmonad.
YggdrasilWM is a learning project for me, so it is **not intended to be used as a daily driver**.

## Dependencies
- cmake
- Xlib
- cxxopts

### Optional
- Xephyr (for testing)
- xinit (for testing)
- xterm (for testing)
- xev (for testing)

## Build
```
git clone https://github.com/corecaps/YggdrasilWM.git
cd YggdrasilWM
mkdir build
cd build
cmake ..
make
```

## Testing using Xephyr
*Do not forget to press Ctrl+Shift in Xephyr Window to test Mouse and Keyboard bindings*
### Using provided start.sh
```
mkdir -p logs
./start.sh
```
logs are stored in the logs directory, the Xephyr window is 800x600 and starts on display :1
### Manually
```
Xephyr -ac -br -noreset -screen 800x600 :1 &
export DISPLAY=:1
xinit ./build/YggdrasilWM
```

## Documentation
The documentation is generated using Doxygen. Run 
```
doxygen doxygen.cfg
```
in the root directory to generate the documentation in the doc/ subdirectory.
Feel free to alterate the doxygen.cfg file to your needs.