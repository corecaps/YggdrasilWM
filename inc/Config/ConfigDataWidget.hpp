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
 * @file ConfigDataWidget.hpp
 * @brief ConfigDataWidget class header
 * @date 2024-02-12
 */
#ifndef CONFIGDATAWIDGET_HPP
#define CONFIGDATAWIDGET_HPP
#include "Config/ConfigDataBase.hpp"
#include "json/json.h"
#include <string>

class ConfigDataWidget : public ConfigDataBase{
public:
	ConfigDataWidget();
	~ConfigDataWidget() override = default;
	void configInit(const Json::Value& root) override;
	Json::Value configSave() override;
	const std::string &getType() const;
	const std::string &getPluginLocation() const;
	const std::string &getFontName() const;
	int getFontSize() const;
	int getBorderSize() const;
	unsigned long getBorderColor() const;
	int getPosition() const;
	int getSize() const;
	unsigned long getFgColor() const;
	unsigned long getBgColor() const;
	const std::string &getArgs() const;

private:
	std::string type;
	std::string pluginLocation;
	std::string fontName;
	int		fontSize;
	int 	borderSize;
	unsigned long borderColor;
	int		position;
	int		size;
	unsigned long fgColor;
	unsigned long bgColor;
	std::string args;
};
#endif // CONFIGDATAWIDGET_HPP