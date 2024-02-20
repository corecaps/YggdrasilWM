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

#include "Bars/Bar.hpp"
#include "Config/ConfigDataBar.hpp"
#include "WindowManager.hpp"
#include <thread>
Bar::Bar() {

}

Bar::~Bar() {

}

void Bar::init(ConfigDataBar *config, TSBarsData *ts) {
	configData = config;
	tsData = ts;
	display = WindowManager::getInstance()->getDisplay();
	int screen = DefaultScreen(display);
	root = RootWindow(display, screen);
	int posX = 0;
	int posY = 0;
	if (configData->getBarPosition() == "top") {
		sizeY = configData->getBarSize();
		sizeX = DisplayWidth(display, screen);
		posX = 0;
		posY = 0;
	} else if (configData->getBarPosition() == "bottom") {
		sizeY = configData->getBarSize();
		sizeX = DisplayWidth(display, screen);
		posX = 0;
		posY = DisplayHeight(display, screen) - sizeY;
	} else if (configData->getBarPosition() == "left") {
		sizeY = DisplayHeight(display, screen);
		sizeX = configData->getBarSize();
		posX = 0;
		posY = 0;
	} else if (configData->getBarPosition() == "right") {
		sizeY = DisplayHeight(display, screen);
		sizeX = configData->getBarSize();
		posX = DisplayWidth(display, screen) - sizeX;
		posY = 0;
	}
	unsigned int bg = configData->getBarBackgroundColor();
	unsigned int fg = configData->getBarFontColor();
	unsigned int border = configData->getBarBorderColor();
	unsigned int borderSize = configData->getBarBorderSize();
	window = XCreateSimpleWindow(display, root, posX, posY,sizeX, sizeY, borderSize, 0, bg);
	XSelectInput(display, window, ExposureMask | KeyPressMask);
	XMapWindow(display, window);
	this->draw();
}

void Bar::draw() {
	int screen = DefaultScreen(display);
	std::stringstream message;
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm localTime{};
	localtime_r(&now, &localTime);
	message << PROGRAM_NAME << " " << PROGRAM_VERSION << " " << WindowManager::getInstance()->getClientCount() << " clients" << " " << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") ;
	XClearWindow(display, window);
	XDrawString(display, window, DefaultGC(display, screen), 300, 15, message.str().c_str(), message.str().size());
	XFlush(display);
}

Window Bar::getWindow() const {
	return window;
}

unsigned int Bar::getSizeX() const {
	return sizeX;
}

unsigned int Bar::getSizeY() const {
	return sizeY;
}
