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
 * @date 2024-02-10
 */
#include "Config/ConfigDataGroups.hpp"
#include "Config/ConfigDataGroup.hpp"
#include "Logger.hpp"
#include <strstream>

ConfigDataGroups::ConfigDataGroups() : groups_(), root_() {}
void ConfigDataGroups::configInit(Json::Value &root) {
	root_ = root;
	if (root_.empty() || !root_.isArray()) {
		throw std::runtime_error("Invalid configuration file");
	}
	for (auto &group : root_) {
		auto group_ = new ConfigDataGroup();
		group_->configInit(group);
		groups_.push_back(group_);
		Logger::GetInstance()->Log("=============================\t\tGroup [" + group_->getGroupName() + "] Configuration loaded ✓",L_INFO);
	}
	Logger::GetInstance()->Log(std::to_string(groups_.size()) + " Groups configuration loaded [✓]",L_INFO);
}
Json::Value ConfigDataGroups::configSave() {
	return Json::Value();
}
ConfigDataGroup *ConfigDataGroups::getGroup(int index) { return groups_[index]; }
void ConfigDataGroups::addGroup(ConfigDataGroup *group) { groups_.push_back(group); }
void ConfigDataGroups::removeGroup(ConfigDataGroup *group) {
	for (auto it = groups_.begin(); it != groups_.end(); ++it) {
		if (*it == group) {
			groups_.erase(it);
			break;
		}
	}
}
const std::vector<ConfigDataGroup *> &ConfigDataGroups::getGroups() const { return groups_;}
ConfigDataGroups::~ConfigDataGroups() {
	for (auto &group : groups_) {
		delete group;
	}
	groups_.clear();
}
