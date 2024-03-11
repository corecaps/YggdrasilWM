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
#ifndef CLOCK_HPP
#define CLOCK_HPP
#include "Bars/Widget.hpp"
#include <string>
#include <ctime>
#include <chrono>
extern "C" {
#include <X11/Xlib.h>
};
class ClockWidget : public Widget
{
	void initialize(Display* display, Window parentWindow, int x, int y, int width, int height) override;
	void draw() override;
	void handleEvent(XEvent& event) override;
	void shutdown() override;
	void setPosition(int x, int y) override;
	void setSize(int width, int height) override;
	void registerDataKey(const std::string& key) override;
	void unregisterDataKey(const std::string& key) override;
	void updateData(const std::string& key, const std::string& value) override;
private:
	Display* display;
	Window parentWindow;
	Window window;
	int x;
	int y;
	int width;
	int height;
	std::string timeFormat;
	std::string font;
	std::string color;
	std::string backgroundColor;
	std::string borderColor;
	std::string borderWidth;
	std::string padding;
	std::string margin;
	std::string position;
	std::string top;
	std::string bottom;
	std::string left;
	std::string right;
	std::string dataKey;
	std::string dataValue;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate;
};

extern "C" Widget* createPlugin() {
	return new ClockWidget();
}
extern "C" void destroyPlugin(Widget* widget) {
	delete widget;
}
#endif // CLOCK_HPP
