# YggdrasilWM 
**Version 0.1.1**

## What is YggdrasilWM?
YggdrasilWM is a tiling window manager for X11. It is written in C++ and uses the Xlib library. It is inspired by dwm and xmonad.

YggdrasilWM is a **learning project** for me, so it is **not intended to be used as a daily driver**.

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
YggdrasilWM is not yet ready to be used as a daily driver, but you can test it using Xephyr.
Xephyr is a nested X server that runs inside your current X server. It is used to test window managers and other X11 programs.
* Do not forget to press Ctrl+Shift in Xephyr Window to test Mouse and Keyboard bindings
### Using provided start.sh
```
./start.sh
```
The script will start Xephyr, YggdrasilWM and some other programs for testing. The logs are stored in the logs directory, the Xephyr window is 800x600 and starts on display :1
- to change the launched programs edit the xinitrc file in the root directory.
- to edit the Xephyr window size and other options edit the start.sh file in the root directory.

### Manually
```
Xephyr -ac -br -noreset -screen 800x600 :1 &
DISPLAY=:1 ; ./build/Yggdrasilwm
```
As there is no .xinitrc file, you will have to start a terminal manually using

```
DISPLAY=:1 ; xterm & 
```

## TODO:
### Tiling Layouts
For the moments only one layout is implemented, the biggest space occupied is splitted along his longest side. The layout is splitted recursively.

### Keybindings and Configuration
The configuration class is yet to be implemented.

### Groups 
Groups are yet to be implemented.

## Documentation
The documentation is generated using Doxygen. Run 
```
doxygen doxygen.cfg
```
in the root directory to generate the documentation in the doc/ subdirectory.
Feel free to edit the doxygen.cfg file to your needs.