| ![gplv3-only](https://www.gnu.org/graphics/gplv3-with-text-136x68.png) | ![YggdrasilWM](logo_small.png) |
|------------------------------------------------------------------------|--------------------------------|
---
# YggdrasilWM 
**Version 0.1.1**
## Documentation
Full Documentation for the project is available [HERE !](https://corecaps.github.io/YggdrasilWM/doc/html/index.html)
## What is YggdrasilWM?
YggdrasilWM is an automatic tiling window manager for X11. It is written in C++ and uses the Xlib library. It is inspired by dwm and xmonad.

YggdrasilWM is a **learning project** for me, so it is **not intended to be used as a daily driver**.

## License
YggdrasilWM is licensed under the GNU General Public License v3. The full GPL license is available in the LICENSE file.

## Dependencies
- C++17 
- cmake https://cmake.org/
- Xlib https://www.x.org/releases/current/doc/libX11/libX11/libX11.html
- cxxopts https://github.com/jarro2783/cxxopts
- jsoncpp https://github.com/open-source-parsers/jsoncpp

### Optional
used for testing :  
- Xephyr https://www.freedesktop.org/wiki/Software/Xephyr/
- xinit 
- xterm https://invisible-island.net/xterm/
- xev 
used for documentation :
- doxygen https://www.doxygen.nl/index.html

## Build
```
git clone https://github.com/corecaps/YggdrasilWM.git
cd YggdrasilWM
mkdir build
cd build
cmake ..
make
```
## Usage
```
Usage: YggdrasilWM [options...]
```
### Options:
```
  -h,--help                   Print this help message and exit
  -v,--version                Print version information and exit
  -d,--display <display>      X11 display to connect to
  -l,--log <log>              Log file
  -c,--config <config>        Config file
```
## Configuration
The configuration file is a JSON file.
The program will look for a file named config.json in this order :
- The file specified with the -c option
- The current directory
- $HOME/.config/yggdrasilwm/config.json
- /etc/yggdrasilwm/config.json

 *Note that the use of jsoncpp library let you use C comment in json even if it's not recognized in the json standard*
```json
{
  // Yggdrasil Window Manager Configuration
  "Groups": [
    // You can put as many groups as wanted but you need at least one
    {
      // Each Group has it's own layout
      "Name": "1",
      "Layout": "Tree",
      "Inactive_Color": "#FF0000",
      "Active_Color": "#00FF00",
      "Border_Size": 2,
      "Gap": 10
    },
    {
      "Name": "2",
      "Layout": "Tree",
      "Inactive_Color": "#FF0000",
      "Active_Color": "#00FF00",
      "Border_Size": 2,
      "Gap": 10
    },
    {
      "Name": "3",
      "Layout": "Tree",
      "Inactive_Color": "#FF0000",
      "Active_Color": "#00FF00",
      "Border_Size": 2,
      "Gap": 10
    }],
  "Bars": [
    // For the moment the bar is not fully implemented
    {
      "Height": 30,
      "Font": "Arial",
      "Font_Size": 12,
      "Font_Color": "#000000",
      "Background_Color": "#FFFFFF",
      "Border_Size": 2,
      "Border_Color": "#000000"
    }
  ],
  "Bindings": {
    // Bindings are groupped by Modifiers
    "Mod1": [
      // Each Bindings must have : 
      // Key: Keysyms string there is a script in util_scripts to extracts strings from your X11 includes in markdown format
      // Action: At the moment only action supported are FocusGroup and Spawn
      // Args: arguments passed to the command object
      // Note that each bindings is grabbed from each managed window watch out for conflicts 
      {
        // Argument to FocusGroup is the group index (starting from 1) not the group name
        "Key" : "1",
        "Action" : "FocusGroup",
        "Argument" : "1"
      },
      {
        "Key" : "2",
        "Action" : "FocusGroup",
        "Argument" : "2"
      },
      {
        "Key" : "3",
        "Action" : "FocusGroup",
        "Argument" : "3"
      },
      // Spawn command object will pass to execvp the first word as the binary and the rest will be build as an argv array
      {
        "Key" : "Return",
        "Action" : "Spawn",
        "Argument" : "kitty"
      }
    ]
  }
}
```

I am still working on the configuration file, so it is not yet fully implemented, and not all options are used. I will update this section when the configuration file is fully implemented.
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
./build/Yggdrasilwm -d :1 -l logs/yggdrasilwm.log --log-level 0
```
As there is no .xinitrc file, you will have to start a terminal manually using

```
DISPLAY=:1 ; xterm & 
```
---
## TODO:
### Tiling Layouts
For the moments only one layout is implemented, the biggest space occupied is split along his longest side. The layout is split recursively.

## Documentation
The documentation is generated using Doxygen. Run 
```
doxygen doxygen.cfg
```
in the root directory to generate the documentation in the doc/ subdirectory.
Feel free to edit the doxygen.cfg file to your needs.

