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
 * @file ConfigDataBar.cpp
 * @brief ConfigDataBar class implementation.
 * @date 2024-02-08
 */

#include "Config/ConfigDataBar.hpp"
#include "Logger.hpp"
#include <strstream>

ConfigDataBar::ConfigDataBar() {

}
void ConfigDataBar::configInit(Json::Value &root_) {
	if (root_.empty() || !root_.isObject()) {
		throw std::runtime_error("Invalid configuration file");
	}
	barHeight_ = root_["Height"].asInt();
	barFont_ = root_["Font"].asString();
//	barFontColor_ = root_["Font_Color"].asUInt();
	barFontSize_ = root_["Font_Size"].asInt();
//	barBackgroundColor_ = root_["Background_Color"].asUInt();
	barBorderSize_ = root_["Border_Size"].asInt();
//	barBorderColor_ = root_["Border_Color"].asUInt();
	std::strstream msg;
	msg << "Bar :\t Height [" << barHeight_ << "] Font [" << barFont_ << "] FontSize [" << barFontSize_ << "] BorderSize [" << barBorderSize_ << "]";
	Logger::GetInstance()->Log(msg.str(),L_INFO);
}
Json::Value ConfigDataBar::configSave() {
	return Json::Value();
}
int ConfigDataBar::getBarHeight() const { return barHeight_; }
const std::string &ConfigDataBar::getBarFont() const {  return barFont_; }
unsigned int ConfigDataBar::getBarFontColor() const { return barFontColor_; }
int ConfigDataBar::getBarFontSize() const { return barFontSize_; }
unsigned int ConfigDataBar::getBarBackgroundColor() const { return barBackgroundColor_; }
int ConfigDataBar::getBarBorderSize() const { return barBorderSize_; }
unsigned int ConfigDataBar::getBarBorderColor() const { return barBorderColor_; }
