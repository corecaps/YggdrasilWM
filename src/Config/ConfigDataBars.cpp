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
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * @file ConfigDataBars.cpp
 * @brief ConfigDataBars class implementation.
 * @date 2024-02-12
 */

#include "Config/ConfigDataBars.hpp"
#include "Config/ConfigDataBar.hpp"
#include "Logger.hpp"

ConfigDataBars::ConfigDataBars() : bars_() {}
void ConfigDataBars::configInit(const Json::Value &root) {
	if (root.empty() || !root.isArray()) {
		throw std::runtime_error("Invalid configuration file");
	}
	for (auto &bar : root) {
		auto *barData = new ConfigDataBar();
		barData->configInit(bar);
		size_t newIndex = bars_.size();
		bars_.emplace_back(barData);
		Logger::GetInstance()->Log("=============================\t\tBar [" + std::to_string(newIndex) + "] Configuration loaded ✓",L_INFO);
	}
	Logger::GetInstance()->Log(std::to_string(bars_.size()) + " Bars configuration loaded [✓]",L_INFO);
}
Json::Value ConfigDataBars::configSave() {
	return Json::Value();
}
ConfigDataBar *ConfigDataBars::getBar(int index) { return bars_[index]; }
void ConfigDataBars::addBar(const std::string &barName, ConfigDataBar *bar) { bars_.emplace_back(bar); }
void ConfigDataBars::removeBar(int index) { bars_.erase(bars_.begin() + index);}
const std::vector<ConfigDataBar *> &ConfigDataBars::getBars() const { return bars_; }

ConfigDataBars::~ConfigDataBars() {
	for (auto &bar : bars_) {
		delete bar;
	}
	bars_.clear();
}
