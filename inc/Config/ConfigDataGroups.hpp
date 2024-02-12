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
 * @file ConfigDataGroups.hpp
 * @brief ConfigDataGroups class header.
 * @date 2024-02-12
 */

#ifndef YGGDRASILWM_CONFIGDATAGROUPS_H
#define YGGDRASILWM_CONFIGDATAGROUPS_H
#include "ConfigDataBase.hpp"
#include "json/json.h"
#include <string>
#include <unordered_map>
#include <vector>
/**
 * @class ConfigDataGroups
 * @brief ConfigDataGroups class.
 * stores an array of ConfigDataGroup created from the config file.
 */
class ConfigDataGroup;
class ConfigDataGroups : public ConfigDataBase {
public:
	ConfigDataGroups();
	~ConfigDataGroups() override;
/**
 * @fn void ConfigDataGroups::configInit(Json::Value& root_)
 * @brief Must be called after instanciation to load the config file.
 * Parses the json root_ and creates ConfigDataGroup objects.
 * @param root_
 */
	void configInit(Json::Value& root_) override;
	Json::Value configSave() override;
	ConfigDataGroup * getGroup(int index);
	void addGroup(ConfigDataGroup * group);
	void removeGroup(ConfigDataGroup * group);
	const std::vector<ConfigDataGroup *> &getGroups() const;

private:
	std::vector<ConfigDataGroup *> groups_;
	Json::Value root_;
};
#endif //YGGDRASILWM_CONFIGDATAGROUPS_H