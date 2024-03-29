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
 * @file ConfigDataGroup.hpp
 * @brief ConfigDataGroup class header.
 * @date 2024-02-12
 */
#ifndef YGGDRASILWM_CONFIGDATAGROUP_HPP
#define YGGDRASILWM_CONFIGDATAGROUP_HPP
#include "ConfigDataBase.hpp"
#include "json/json.h"
#include <string>
/**
 * @class ConfigDataGroup
 * @brief ConfigDataGroup class
 * Store the configuration for a group from the config file.
 * the logic of groups is handled by the Group class
 * the logic of the layout of a group is handled in
 * a sub class of LayoutManager class e.g. TreeLayoutManager
 */
class ConfigDataGroup : public ConfigDataBase {
public:
			ConfigDataGroup();
			~ConfigDataGroup() override = default;
/**
 * @fn void ConfigDataGroup::configInit(Json::Value& root)
 * @brief Initialize the configuration for a group from the config file.
 * must be called after instanciating the object.
 * Parse the Json::Value root and store the values in the object.
 * @param root
 */
	void	configInit(const Json::Value& root) override;
	Json::Value							configSave() override;
	[[nodiscard]] const std::string &	getGroupName() const;
	[[nodiscard]] const std::string &	getGroupLayout() const;
	[[nodiscard]] unsigned long			getGroupInactiveColor() const;
	[[nodiscard]] unsigned long			getGroupActiveColor() const;
	[[nodiscard]] int					getGroupBorderWidth() const;
	[[nodiscard]] int					getGroupGap() const;

private:
	std::string		groupName_;
	std::string		groupLayout_;
	unsigned long	groupInactiveColor_{};
	unsigned long	groupActiveColor_{};
	int				groupBorderSize_{};
	int				groupGap_{};
};
#endif //YGGDRASILWM_CONFIGDATAGROUP_HPP