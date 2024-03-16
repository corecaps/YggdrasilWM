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
 * @file ConfigDataBar.cpp
 * @brief ConfigDataBar class implementation.
 * @date 2024-02-12
 */

#include "Config/ConfigDataBar.hpp"
#include "Config/ConfigHandler.hpp"
#include "Logger.hpp"
#include "Config/ConfigDataWidget.hpp"
#include <memory>
#include <vector>
#include <sstream>

ConfigDataBar::ConfigDataBar() :
		barSize_(20),
		barFont_("Arial"),
		barFontColor_(0x00ff00),
		barFontSize_(12),
		barBackgroundColor_(0x00ff00),
		barBorderSize_(2),
		barBorderColor_(0x00ff00) {

}
void ConfigDataBar::configInit(const Json::Value &root) {
	if (root.empty() || !root.isObject()) {
		throw std::runtime_error("Invalid configuration file");
	}
	if (root["Bar_Size"].empty() || !root["Bar_Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataBar::Bar_Size is empty or not an integer",L_ERROR);
	} else {
		barSize_ = root["Bar_Size"].asInt();
	}
	if (root["Font"].empty() || !root["Font"].isString()) {
		Logger::GetInstance()->Log("ConfigDataBar::Font is empty or not a string", L_ERROR);
	} else {
		barFont_ = root["Font"].asString();
	}
	if (root["Font_Color"].empty() || !root["Font_Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataBar::Font_Color is empty or not a string",L_ERROR);
	} else
	{
		barFontColor_ = ConfigHandler::colorCodeToULong(root["Font_Color"].asString());
	}
	if (root["Background_Color"].empty() || !root["Background_Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataBar::Background_Color is empty or not a string",L_ERROR);
	} else
	{
		barBackgroundColor_ = ConfigHandler::colorCodeToULong(root["Background_Color"].asString());
	}
	if (root["Border_Color"].empty() || !root["Border_Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataBar::Border_Color is empty or not a string",L_ERROR);
	} else
	{
		barBorderColor_ = ConfigHandler::colorCodeToULong(root["Border_Color"].asString());
	}
	if (root["Font_Size"].empty() || !root["Font_Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataBar::Font_Size is empty or not an integer",L_ERROR);
	} else {
		barFontSize_ = root["Font_Size"].asInt();
	}
	if (root["Border_Size"].empty() || !root["Border_Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataBar::Border_Size is empty or not an integer",L_ERROR);
	} else {
		barBorderSize_ = root["Border_Size"].asInt();
	}
	if (root["Position"].empty() || !root["Position"].isString()) {
		Logger::GetInstance()->Log("ConfigDataBar::Position is empty or not a string",L_ERROR);
	} else {
		barPosition_ = root["Position"].asString();
	}
	for (auto &widget : root["Widgets"]) {
		std::shared_ptr<ConfigDataWidget> widgetData = std::make_shared<ConfigDataWidget>();
		widgetData->configInit(widget);
		widgets.push_back(widgetData);
	}
	std::stringstream msg;
	msg << "Bar :\t Height [" << barSize_ << "] Font [" << barFont_ << "] FontSize [" << barFontSize_ << "] BorderSize [" << barBorderSize_ << "]";
	Logger::GetInstance()->Log(msg.str(),L_INFO);
}
Json::Value ConfigDataBar::configSave() {
	return Json::Value();
}
int ConfigDataBar::getBarSize() const { return barSize_; }
const std::string &ConfigDataBar::getBarFont() const {  return barFont_; }
unsigned int ConfigDataBar::getBarFontColor() const { return barFontColor_; }
int ConfigDataBar::getBarFontSize() const { return barFontSize_; }
unsigned int ConfigDataBar::getBarBackgroundColor() const { return barBackgroundColor_; }
int ConfigDataBar::getBarBorderSize() const { return barBorderSize_; }
unsigned int ConfigDataBar::getBarBorderColor() const { return barBorderColor_; }
const std::string &ConfigDataBar::getBarPosition() const { return barPosition_; }

const std::vector<std::shared_ptr<ConfigDataWidget>> &ConfigDataBar::getWidgets() const {
	return widgets;
}
