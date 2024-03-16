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
#include "Config/ConfigDataWidget.hpp"
#include "WindowManager.hpp"
#include <dlfcn.h>
#include "Bars/Widget.hpp"

Bar::Bar() : window(0),
			 sizeX(0),
			 sizeY(0),
			 display(nullptr),
			 root(0) {}

Bar::~Bar() {
//	typedef void destroy_t(Widget*);
//	destroy_t *destroyPlugin = (destroy_t*)dlsym(handle, "destroyPlugin");
//	if (!destroyPlugin) {
//		std::cerr << "Cannot load symbol destroyPlugin: " << dlerror() << '\n';
//		dlclose(handle);
//		return;
//	}
//	destroyPlugin(widget);
}

void Bar::init(std::shared_ptr<ConfigDataBar> configData, std::shared_ptr<TSBarsData> tsData) {
	configData = configData;
	tsData = tsData;
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
	XSelectInput(display, window,SubstructureRedirectMask
								 | SubstructureNotifyMask
								 | FocusChangeMask
								 | KeyPressMask
								 | ExposureMask
								 | ButtonPressMask
								 | PointerMotionMask);
	XMapWindow(display, window);
	this->draw();
}

void Bar::draw() {
	if (widgets.empty()) {
		return;
	}
	for (auto &w : widgets) {
		w.second->draw();
	}
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

void Bar::addWidget(void *handle, std::shared_ptr<ConfigDataWidget> widgetConfig) {
	typedef Widget* create_t();
	create_t* createPlugin = (create_t*)dlsym(handle, "createPlugin");
	if (!createPlugin) {
		std::cerr << "Cannot load symbol createPlugin: " << dlerror() << '\n';
		return;
	}
	Widget * newWidget = createPlugin();
	if (!newWidget) {
		Logger::GetInstance()->Log("Cannot create plugin: " + std::string(dlerror()), L_ERROR);
		return;
	}
	// TODO : change position/size for left/right bar
	Window newWidgetWindow = newWidget->initialize(display,
										window,
										widgetConfig->getPosition(),0,
										widgetConfig->getSize(),(int)sizeY,
										widgetConfig->getFontName(),
										widgetConfig->getBgColor(),
										widgetConfig->getFgColor(),
										widgetConfig->getFontSize());
	Logger::GetInstance()->Log("New Widget added ["
							   + widgetConfig->getType()
							   + "] widgetWindow: ["
							   + std::to_string(newWidgetWindow)
							   + "] Bar ["
							   + std::to_string(window)
							   + "]", L_INFO);
	widgets[newWidgetWindow] = newWidget;
}

const std::unordered_map<Window, Widget *> &Bar::getWidgets() const {
	return widgets;
}
