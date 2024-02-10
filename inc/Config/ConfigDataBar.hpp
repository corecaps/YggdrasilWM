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
 * @date 2024-02-10
 */
#ifndef YGGDRASILWM_CONFIGDATAGROUP_H
#define YGGDRASILWM_CONFIGDATAGROUP_H
#include "Config/ConfigDataBase.hpp"
#include "json/json.h"
#include <string>

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
 * @param root_ Json::Value& object containing the Bar configuration data
 */
	void configInit(Json::Value& root_) override;
/**
 * @fn Json::Value ConfigDataBar::configSave()
 * @brief return a Json::Value object containing the Bar configuration data
 */
	Json::Value configSave() override;
/**
 * @fn int ConfigDataBar::getBarHeight() const
 * @brief Get the Bar Heightb
 * @return
 */
	int getBarHeight() const;
/**
 * @fn const std::string &ConfigDataBar::getBarFont() const
 * @brief Get the Bar Font
 */
	const std::string &getBarFont() const;
/**
 * @fn unsigned int ConfigDataBar::getBarFontColor() const
 * @brief Get the Bar Font Color
 */
	unsigned int getBarFontColor() const;
/**
 * @fn int ConfigDataBar::getBarFontSize() const
 * @brief Get the Bar Font Size
 */
	int getBarFontSize() const;
/**
 * @fn unsigned int ConfigDataBar::getBarBackgroundColor() const
 * @brief Get the Bar Background Color
 */
	unsigned int getBarBackgroundColor() const;
/**
 * @fn int ConfigDataBar::getBarBorderSize() const
 * @brief Get the Bar Border Size
 */
	int getBarBorderSize() const;
/**
 * @fn unsigned int ConfigDataBar::getBarBorderColor() const
 * @brief Get the Bar Border Color
 */
	unsigned int getBarBorderColor() const;
private:
	int barHeight_;
	std::string barFont_;
	unsigned int barFontColor_;
	int barFontSize_;
	unsigned int barBackgroundColor_;
	int barBorderSize_;
	unsigned int barBorderColor_;
};

#endif //YGGDRASILWM_CONFIGDATAGROUP_H