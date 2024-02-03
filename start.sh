# Yb  dP              8                w 8 Yb        dP 8b   d8
#  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
#   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
#   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
#        wwdP wwdP
# Yggdrasil Window Manager
# https://github.com/corecaps/YggdrasilWM
# Copyright (C) 2024 jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
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
xinit ./xinitrc -- /usr/bin/Xephyr :1 -ac -screen 800x600 -host-cursor
cat logs/std-logs.txt
