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
 * @file ConfigDataGroups.cpp
 * @brief ConfigDataGroups class implementation.
 * @date 2024-02-08
 */
#include "Config/ConfigDataGroups.hpp"
#include "Config/ConfigDataGroup.hpp"
#include "Logger.hpp"
#include <strstream>

ConfigDataGroups::ConfigDataGroups() : groups_(), root_() {}
void ConfigDataGroups::configInit(Json::Value &root_) {
	root_ = root_;
	if (root_.empty() || !root_.isObject()) {
		throw std::runtime_error("Invalid configuration file");
	}
	for (auto const &groupName : root_.getMemberNames()) {
		auto group = new ConfigDataGroup();
		group->configInit(root_[groupName]);
		groups_[groupName] = group;
		Logger::GetInstance()->Log("=============================\t\tGroup [" + groupName + "] Configuration loaded ✓",L_INFO);
	}
	Logger::GetInstance()->Log(std::to_string(groups_.size()) + " Groups configuration loaded [✓]",L_INFO);
}
Json::Value ConfigDataGroups::configSave() {
	return Json::Value();
}
ConfigDataGroup *ConfigDataGroups::getGroup(const std::string &groupName) { return groups_[groupName]; }
void ConfigDataGroups::addGroup(const std::string &groupName, ConfigDataGroup *group) { groups_[groupName] = group; }
void ConfigDataGroups::removeGroup(const std::string &groupName) { groups_.erase(groupName); }
const std::unordered_map<std::string, ConfigDataGroup *> &ConfigDataGroups::getGroups() const { return groups_;}

ConfigDataGroups::~ConfigDataGroups() {
	for (auto const &group : groups_) {
		delete group.second;
	}
	groups_.clear();
}
