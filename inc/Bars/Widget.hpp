/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * https://github.com/corecaps/YggdrasilWM
 * Copyright (C) 2024 jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef WIDGET_HPP
#define WIDGET_HPP
extern "C" {
#include <X11/Xlib.h>
};
#include <string>

class Widget
{
public:
	Widget() = default;
	virtual ~Widget() = 0;
	virtual void initialize(Display* display, Window parentWindow, int x, int y, int width, int height) = 0;
	virtual void draw() = 0;
	virtual void handleEvent(XEvent& event) = 0;
	virtual void shutdown() = 0;
	virtual void setPosition(int x, int y) = 0;
	virtual void setSize(int width, int height) = 0;
	virtual void registerDataKey(const std::string& key) = 0;
	virtual void unregisterDataKey(const std::string& key) = 0;
	virtual void updateData(const std::string& key, const std::string& value) = 0;
};
#endif // WIDGET_HPP