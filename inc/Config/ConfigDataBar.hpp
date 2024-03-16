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
 * @file ConfigDataBar.hpp
 * @brief ConfigDataBar class header
 * @date 2024-02-12
 */
#ifndef YGGDRASILWM_CONFIGDATAGROUP_HPP
#define YGGDRASILWM_CONFIGDATAGROUP_HPP
#include "Config/ConfigDataBase.hpp"
#include "json/json.h"
#include <string>
#include <vector>
class ConfigDataWidget;
/**
 * @class ConfigDataBar
 * @brief ConfigDataBar class
 * Contains the configuration data for one bar in the window manager
 * Inherits from ConfigDataBase class
 * @see ConfigDataBase
 */
class ConfigDataBar : public ConfigDataBase{
public:
/**
 * @fn ConfigDataBar::ConfigDataBar()
 * @brief ConfigDataBar class constructor
 */
	ConfigDataBar();
	~ConfigDataBar() override = default;
/**
 * @fn void ConfigDataBar::configInit(Json::Value& root_)
 * @brief Initializes the configuration data for the bar
 * You must call configInit() in each ConfigDataBase Child class
 * to load the data.
 * @param root Json::Value& object containing the Bar configuration data
 */
	void configInit(const Json::Value& root) override;
/**
 * @fn Json::Value ConfigDataBar::configSave()
 * @brief return a Json::Value object containing the Bar configuration data
 */
	Json::Value configSave() override;
/**
 * @fn int ConfigDataBar::getBarSize() const
 * @brief Get the Bar Heightb
 * @return
 */
	[[nodiscard]] int getBarSize() const;
/**
 * @fn const std::string &ConfigDataBar::getBarFont() const
 * @brief Get the Bar Font
 */
	[[nodiscard]] const std::string &getBarFont() const;
/**
 * @fn unsigned int ConfigDataBar::getBarFontColor() const
 * @brief Get the Bar Font Color
 */
	[[nodiscard]] unsigned int getBarFontColor() const;
/**
 * @fn int ConfigDataBar::getBarFontSize() const
 * @brief Get the Bar Font Size
 */
	[[nodiscard]] int getBarFontSize() const;
/**
 * @fn unsigned int ConfigDataBar::getBarBackgroundColor() const
 * @brief Get the Bar Background Color
 */
	[[nodiscard]] unsigned int getBarBackgroundColor() const;
/**
 * @fn int ConfigDataBar::getBarBorderSize() const
 * @brief Get the Bar Border Size
 */
	[[nodiscard]] int getBarBorderSize() const;
/**
 * @fn unsigned int ConfigDataBar::getBarBorderColor() const
 * @brief Get the Bar Border Color
 */
	[[nodiscard]] unsigned int getBarBorderColor() const;
	[[nodiscard]] const std::string &getBarPosition() const;
	const std::vector<std::shared_ptr<ConfigDataWidget>> &getWidgets() const;

private:
	int				barSize_;
	std::string		barFont_;
	std::string		barPosition_;
	unsigned int	barFontColor_;
	int				barFontSize_;
	unsigned int	barBackgroundColor_;
	int				barBorderSize_;
	unsigned int	barBorderColor_;
	std::vector<std::shared_ptr<ConfigDataWidget>> widgets;

};

#endif //YGGDRASILWM_CONFIGDATAGROUP_HPP