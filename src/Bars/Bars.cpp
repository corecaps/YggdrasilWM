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

#include "Bars/Bars.hpp"
#include "Bars/Bar.hpp"
#include "Config/ConfigDataBars.hpp"
#include "Config/ConfigDataBar.hpp"
#include "Config/ConfigDataWidget.hpp"
#include "Bars/TSBarsData.hpp"
#include "Bars/Widget.hpp"
#include "WindowManager.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <dlfcn.h>

Bars * Bars::instance = nullptr;
void Bars::init(std::shared_ptr<ConfigDataBars> configData,
				std::shared_ptr<TSBarsData> tsData,
				Display *display,
				Window root) {
	this->configData = configData;
	this->tsData =tsData;
	this->display = display;
	this->root = root;
	for (auto &bar : this->configData->getBars()) {
		std::unique_ptr<Bar> newBar = std::make_unique<Bar>();
		newBar->init(bar, this->tsData);
		if (bar->getBarPosition() == "top") {
			this->spaceN += bar->getBarSize();
		} else if (bar->getBarPosition() == "bottom") {
			this->spaceS += bar->getBarSize();
		} else if (bar->getBarPosition() == "left") {
			this->spaceW += bar->getBarSize();
		} else if (bar->getBarPosition() == "right") {
			this->spaceE += bar->getBarSize();
		}
		for (auto &w : bar->getWidgets()) {
			if (pluginsLocations.find(w->getPluginLocation()) == pluginsLocations.end()) {
				addPluginLocation(w->getPluginLocation());
				void *handle = dlopen(w->getPluginLocation().c_str(), RTLD_LAZY);
				if (!handle) {
					Logger::GetInstance()->Log("Cannot open library: " + std::string(dlerror()),L_ERROR);
					continue;
				}
				setWidgetTypeHandle(w->getType(), handle);
				Logger::GetInstance()->Log("Library " + w->getPluginLocation() + " opened",L_INFO);
			}
			newBar->addWidget(getWidgetTypeHandle(w->getType()), w);
		}
		Logger::GetInstance()->Log("Bar ["
									+ std::to_string(this->bars.size())
									+ "] on window ["
									+ std::to_string(newBar->getWindow())
									+ "] initialized\t"
									+ bar->getBarPosition()
									+ " "
									+ std::to_string(newBar->getSizeX())
									+ " x "
									+ std::to_string(newBar->getSizeY()),L_INFO);
		for (auto w:newBar->getWidgets()) {
			subscribeWidget(w.second);
		}
		this->windows.push_back(newBar->getWindow());
		this->bars.push_back(std::move(newBar));
	}
}
void Bars::start_thread() {
	barThread = std::thread(&Bars::run, this);
}
void Bars::run() {
	while (WindowManager::getInstance()->getRunning()){
		try {
			if (tsData->wait()) {
				// Updated Data redraw
				// Todo check for registerd keys to redraw only concerned widgets
				std::unordered_map<std::string,std::string>updated = tsData->getData();
				for (const auto &pair: updated) {
					this->data[pair.first] = pair.second;
					if (subscriptions.find(pair.first) != subscriptions.end()) {
						Logger::GetInstance()->Log("Looking subscriber for " + pair.first ,L_INFO);
						for (Widget* widget : subscriptions[pair.first]) {
							Logger::GetInstance()->Log("Found Subscriber : " + std::to_string((unsigned long int)widget),L_INFO);
							widget->updateData(pair.first, pair.second);
							widget->draw();
						}
					}
					XFlush(display);
					Logger::GetInstance()->Log("Updated Data: " + pair.first + " : " + pair.second,L_INFO);
				}
				this->redraw();
			} else {
				// Periodic redraw
				this->redraw();
			}
		} catch (const std::exception &e) {
			Logger::GetInstance()->Log("Bars thread exception: " + std::string(e.what()),L_ERROR);
		}
	}
}
void Bars::redraw() {
	for (auto &bar : this->bars) {
		bar->draw();
	}
}
Bars::Bars() : spaceN(0),
			   spaceS(0),
			   spaceE(0),
			   spaceW(0),
			   configData(nullptr),
			   tsData(nullptr),
			   data(),
			   display(nullptr),
			   root(0)
				{}

void Bars::addPluginLocation(const std::string &location) {
	pluginsLocations.emplace(location);
}

const std::set<std::string> &Bars::getPluginsLocations() const {
	return pluginsLocations;
}

void *Bars::getWidgetTypeHandle(const std::string &widgetType) {
	try {
		void * handle = widgetTypeHandle[widgetType];
		return handle;
	} catch (std::out_of_range &e) {
		Logger::GetInstance()->Log("Widget Type " + widgetType + " not found",L_ERROR);
		return nullptr;
	}
}

void Bars::setWidgetTypeHandle(const std::string &widgetType, void *handle) {
	widgetTypeHandle[widgetType] = handle;
}
Bars::~Bars() {
	for (auto &bar : bars) {
		for (auto &w : bar->getWidgets()) {
			typedef void (*destroy_t)(Widget *);
			destroy_t destroy = (destroy_t)dlsym(w.second, "destroyPlugin");
			if (!destroy) {
				Logger::GetInstance()->Log("Cannot load symbol destroy: " + std::string(dlerror()),L_ERROR);
				continue;
			}
			destroy(w.second);
		}
	}
	for (auto handle : widgetTypeHandle) {
		dlclose(handle.second);
	}
}
void Bars::createInstance() {
	if (Bars::instance == nullptr)
		Bars::instance = new Bars();
}
Bars &Bars::getInstance() {
	if (Bars::instance == nullptr)
		Bars::createInstance();
	return *Bars::instance;
}
void Bars::destroy() {
	if (Bars::instance != nullptr)
		delete Bars::instance;
}
unsigned int Bars::getSpaceN() const { return this->spaceN; }
unsigned int Bars::getSpaceS() const { return this->spaceS; }
unsigned int Bars::getSpaceE() const { return this->spaceE; }
unsigned int Bars::getSpaceW() const { return this->spaceW; }
const std::unordered_map<std::string, std::string> &Bars::getData() const { return data; }
bool Bars::isBarWindow(Window window) {
	auto it = std::find(windows.begin(), windows.end(), window);
	return it != windows.end();
}
void Bars::stop_thread() {
	barThread.join();
}

void Bars::subscribeWidget(Widget *w) {
	std::vector<std::string> keys = w->registerDataKey();
	for (const auto &key: keys) {
		if (key.empty())
			continue;
		Logger::GetInstance()->Log("Subscribing widget"+ std::to_string((unsigned long int)w)+" to key: " + key,L_INFO);
		subscriptions[key].push_back(w);
	}
}
