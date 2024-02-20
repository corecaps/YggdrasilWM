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
 * @file ConfigDataGroup.cpp
 * @brief ConfigDataGroup class implementation.
 * @date 2024-02-08
 */
#include "Config/ConfigDataGroup.hpp"
#include "Logger.hpp"
#include "Config/ConfigHandler.hpp"
#include <sstream>
#include <iomanip>

ConfigDataGroup::ConfigDataGroup() {
	groupName_ = "Default";
	groupLayout_ = "Tree";
	groupInactiveColor_ = 0x00ff00;
	groupActiveColor_ = 0x00ff00;
	groupBorderSize_ = 0;
	groupGap_ = 10;
}
void ConfigDataGroup::configInit(const Json::Value &root) {
	if (root.empty() || !root.isObject()) {
		throw std::runtime_error("ConfigDataGroup::configInit root is empty or not an object");
	}
	if (root["Name"].empty() || !root["Name"].isString()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Name is empty or not a string",L_ERROR);
	} else {
		groupName_ = root["Name"].asString();
	}
	if (root["Layout"].empty() || !root["Layout"].isString()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Layout is empty or not a string",L_ERROR);
	} else {
		groupLayout_ = root["Layout"].asString();
	}
	if (root["Inactive_Color"].empty() || !root["Inactive_Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Inactive_Color is empty or not a string",L_ERROR);
	} else
	{
		groupInactiveColor_ = ConfigHandler::colorCodeToULong(root["Inactive_Color"].asString());
	}
	if (root["Active_Color"].empty() || !root["Active_Color"].isString()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Active_Color is empty or not a string",L_ERROR);
	} else
	{
		groupActiveColor_ = ConfigHandler::colorCodeToULong(root["Active_Color"].asString());
	}
	if (root["Border_Size"].empty() || !root["Border_Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Border_Size is empty or not an int",L_ERROR);
	} else {
		groupBorderSize_ = root["Border_Size"].asInt();
	}
	if (root["Gap"].empty() || !root["Gap"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Gap is empty or not an int",L_ERROR);
	} else {
		groupGap_ = root["Gap"].asInt();
	}
	std::ostringstream msg;
	msg << "Group[" << groupName_ << "]"
	<< "\tLayout [" << groupLayout_ << "]"
	<< "\tInactive_Color [#" << std::hex << std::setw(6) << std::setfill('0') << groupInactiveColor_ << std::dec << "]"
	<< "\tActive_Color [#" << std::hex <<  std::setw(6) << std::setfill('0') << groupActiveColor_ << std::dec << "]"
	<< "\tBorder_Size [" << groupBorderSize_ << "]"
	<< "\tGap [" << groupGap_ << "]"
	<< "-> loaded\t\t[✓]";
	Logger::GetInstance()->Log(msg.str(),L_INFO);

}
Json::Value ConfigDataGroup::configSave() {
	return Json::Value();
}
const std::string &ConfigDataGroup::getGroupName() const { return groupName_; }
const std::string &ConfigDataGroup::getGroupLayout() const { return groupLayout_; }
unsigned long ConfigDataGroup::getGroupInactiveColor() const { return groupInactiveColor_; }
unsigned long ConfigDataGroup::getGroupActiveColor() const { return groupActiveColor_; }
int ConfigDataGroup::getGroupBorderWidth() const { return groupBorderSize_; }
int ConfigDataGroup::getGroupGap() const { return groupGap_; }
