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
#include <sstream>
ConfigDataGroup::ConfigDataGroup() {
	groupName_ = "Default";
	groupLayout_ = "Tree";
	groupInactiveColor_ = 0x00ff00;
	groupActiveColor_ = 0x00ff00;
	groupBorderSize_ = 0;
	groupGap_ = 10;
}
void ConfigDataGroup::configInit(Json::Value &root_) {
	root_ = root_;
	if (root_.empty() || !root_.isObject()) {
		throw std::runtime_error("ConfigDataGroup::configInit root is empty or not an object");
	}
	if (root_["Name"].empty() || !root_["Name"].isString()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Name is empty or not a string",L_ERROR);
	} else {
		groupName_ = root_["Name"].asString();
	}
	if (root_["Layout"].empty() || !root_["Layout"].isString()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Layout is empty or not a string",L_ERROR);
	} else {
		groupLayout_ = root_["Layout"].asString();
	}
////	if (root_["Inactive_Color"].empty() || !root_["Inactive_Color"].isString()) {
////		Logger::GetInstance()->Log("ConfigDataGroup::configInit Inactive_Color is empty or not a string",L_ERROR);
////	} else {
////		groupInactiveColor_ = root_["Inactive_Color"].asUInt();
////	}
//	groupInactiveColor_ = std::stoul(root_["Inactive_Color"].asString(), nullptr, 16);
//	groupActiveColor_ = std::stoul(root_["Active_Color"].asString(), nullptr, 16);
	if (root_["Border_Size"].empty() || !root_["Border_Size"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Border_Size is empty or not an int",L_ERROR);
	} else {
		groupBorderSize_ = root_["Border_Size"].asInt();
	}
	if (root_["Gap"].empty() || !root_["Gap"].isInt()) {
		Logger::GetInstance()->Log("ConfigDataGroup::configInit Gap is empty or not an int",L_ERROR);
	} else {
		groupGap_ = root_["Gap"].asInt();
	}
	std::ostringstream msg;
	msg << "Group[" << groupName_ << "]"
	<< "\tLayout [" << groupLayout_ << "]"
//	<< "\tInactive_Color [" << groupInactiveColor_ << "]"
//	<< "\tActive_Color [" << groupActiveColor_ << "]"
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
