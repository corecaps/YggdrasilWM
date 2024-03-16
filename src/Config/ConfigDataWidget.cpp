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
 * @file ConfigDataWidget.cpp
 * @brief ConfigDataWidget class implementation
 * @date 2024-02-12
 */
#include "Config/ConfigDataWidget.hpp"
#include "Logger.hpp"
#include "Config/ConfigHandler.hpp"

ConfigDataWidget::ConfigDataWidget() :
		type(""),
		pluginLocation(""),
		fontName(""),
		fontSize(0),
		borderSize(0),
		borderColor(0),
		position(0),
		size(0),
		fgColor(0),
		bgColor(0),
		args("") {}

void ConfigDataWidget::configInit(const Json::Value &root) {
	if (root.empty() || !root.isObject()) {
		throw std::runtime_error("Invalid configuration file");
	}
	if (root["Type"].empty() || !root["Type"].isString()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Type is empty or not a string",L_ERROR);
	} else {
		type = root["Type"].asString();
	}
	if (root["Plugin"].empty() || !root["Plugin"].isString()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Plugin_Location is empty or not a string", L_ERROR);
	} else {
		pluginLocation = root["Plugin"].asString();
	}
	if (root["Font"].empty() || !root["Font"].isString()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Font_Name is empty or not a string",L_ERROR);
	} else {
		fontName = root["Font"].asString();
	}
	if (root["Font_Size"].empty() || !root["Font_Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Font_Size is empty or not an integer",L_ERROR);
	} else {
		fontSize = root["Font_Size"].asInt();
	}
	if (root["Border_Size"].empty() || !root["Border_Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Border_Size is empty or not an integer",L_ERROR);
	} else {
		borderSize = root["Border_Size"].asInt();
	}
	if (root["Border_Color"].empty() || !root["Border_Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Border_Color is empty or not a string",L_ERROR);
	} else {
		borderColor = ConfigHandler::colorCodeToULong(root["Border_Color"].asString());
	}
	if (root["Position"].empty() || !root["Position"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Position is empty or not an integer",L_ERROR);
	} else {
		position = root["Position"].asInt();
	}
	if (root["Size"].empty() || !root["Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Size is empty or not an integer",L_ERROR);
	} else {
		size = root["Size"].asInt();
	}
	if (root["Color"].empty() || !root["Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Foreground_Color is empty or not a string",L_ERROR);
	} else {
		fgColor = ConfigHandler::colorCodeToULong(root["Color"].asString());
	}
	if (root["Background_Color"].empty() || !root["Background_Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Background_Color is empty or not a string",L_ERROR);
	} else {
		bgColor = ConfigHandler::colorCodeToULong(root["Background_Color"].asString());
	}
	if (!root["Arguments"].isString()) {
		Logger::GetInstance()->Log("ConfigDataWidget::Args is empty or not a string",L_ERROR);
	} else {
		args = root["Arguments"].asString();
	}
}

Json::Value ConfigDataWidget::configSave() {
	return Json::Value();
}

const std::string &ConfigDataWidget::getType() const { return type; }
const std::string &ConfigDataWidget::getPluginLocation() const { return pluginLocation; }
const std::string &ConfigDataWidget::getFontName() const { return fontName; }
int ConfigDataWidget::getFontSize() const { return fontSize; }
int ConfigDataWidget::getBorderSize() const { return borderSize; }
unsigned long ConfigDataWidget::getBorderColor() const { return borderColor; }
int ConfigDataWidget::getPosition() const { return position; }
int ConfigDataWidget::getSize() const { return size; }
unsigned long ConfigDataWidget::getFgColor() const { return fgColor; }
unsigned long ConfigDataWidget::getBgColor() const { return bgColor; }
const std::string &ConfigDataWidget::getArgs() const { return args; }
