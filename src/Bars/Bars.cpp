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
#include "Bars/TSBarsData.hpp"
#include "WindowManager.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
Bars * Bars::instance = nullptr;
void Bars::init(ConfigDataBars *configD,
				std::shared_ptr<TSBarsData> tsD,
				Display *dspl,
				Window r) {
	this->configData = configD;
	this->tsData =tsD;
	this->display = dspl;
	this->root = r;
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
				std::unordered_map<std::string,std::string>updated = tsData->getData();
				std::string last;
				for (const auto &pair: updated) {
					this->data[pair.first] = pair.second;
					last = pair.second;
				}
				this->redraw(last);
			} else {
				this->redraw("TimneOut Redraw");
			}
		} catch (const std::exception &e) {
			Logger::GetInstance()->Log("Bars thread exception: " + std::string(e.what()),L_ERROR);
		}
	}
}
void Bars::redraw(std::string msg) {
	for (auto &bar : this->bars) {
		bar->draw(msg);
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
Bars::~Bars() {
	if (this->configData != nullptr)
		delete this->configData;
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
