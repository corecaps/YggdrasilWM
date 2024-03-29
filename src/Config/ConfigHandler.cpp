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
 * @file ConfigHandler.cpp
 * @brief ConfigHandler class implementation.
 * @date 2024-02-11
 *
 */

#include "Config/ConfigHandler.hpp"
#include "Config/ConfigFileHandler.hpp"
#include "Config/ConfigDataGroups.hpp"
#include "Config/ConfigDataBars.hpp"
#include "Config/ConfigDataBindings.hpp"
#include "Logger.hpp"
ConfigHandler * ConfigHandler::instance_ = nullptr;
void ConfigHandler::Create(const std::string& configPath) {
	if (instance_ == nullptr) {
		instance_ = new ConfigHandler(configPath);
	}
}
void ConfigHandler::Create() {
	if (instance_ == nullptr) {
		instance_ = new ConfigHandler();
	}
}
ConfigHandler& ConfigHandler::GetInstance() {
	if (instance_ == nullptr) {
		throw std::runtime_error("ConfigHandler instance not created");
	}
	return *instance_;
}
void ConfigHandler::Destroy() {
	if (instance_ != nullptr) {
		delete instance_;
		instance_ = nullptr;
	}
}
ConfigHandler::ConfigHandler() :
	configPath_(){
}
ConfigHandler::ConfigHandler(const std::string& configPath) :
	configPath_(configPath){
}

void ConfigHandler::configInit() {
	std::unique_ptr<ConfigFileHandler> configFileHandler_;
	if (configPath_.empty()) {
		configFileHandler_ = std::make_unique<ConfigFileHandler>(ConfigFileHandler());
	} else {
		configFileHandler_ = std::make_unique<ConfigFileHandler>(ConfigFileHandler(configPath_));
	}
	Logger::GetInstance()->Log("===================Loading Config===================",L_INFO);
	configFileHandler_->readConfig();
	configPath_ = configFileHandler_->getConfigPath();
	const Json::Value &root_ = configFileHandler_->getRoot();
	if (root_.empty() || !root_.isObject()) {
		throw std::runtime_error("Config file is empty or not an object");
	}
	Json::Value Groups = root_["Groups"];
	Json::Value Bars = root_["Bars"];
	Json::Value Bindings = root_["Bindings"];
	if (Groups.empty() || !Groups.isArray() || Bars.empty() || !Bars.isArray() || Bindings.empty() || !Bindings.isObject()){
		throw std::runtime_error("Config file is missing Groups, Bars or Bindings See Documentation for more information");
	}
	addConfigData(std::make_shared<ConfigDataGroups>());
	addConfigData(std::make_shared<ConfigDataBars>());
	addConfigData(std::make_shared<ConfigDataBindings>());
	getConfigData<ConfigDataGroups>()->configInit(Groups);
	getConfigData<ConfigDataBars>()->configInit(Bars);
	getConfigData<ConfigDataBindings>()->configInit(Bindings);
	Logger::GetInstance()->Log("===================Succefully Loaded Config===================\n\n",L_INFO);
}
ConfigHandler::~ConfigHandler() = default;
unsigned long ConfigHandler::colorCodeToULong(const std::string& colorCode) {
	if (colorCode.size() != 7 || colorCode[0] != '#' || !isxdigit(colorCode[1])) {
		throw std::runtime_error("Invalid color code");
	}
	std::istringstream iss(colorCode.substr(1));
	unsigned long colorValue;
	iss >> std::hex >> colorValue;
	if (iss.fail()) { throw std::runtime_error("Invalid color code");}
	return colorValue;
}

