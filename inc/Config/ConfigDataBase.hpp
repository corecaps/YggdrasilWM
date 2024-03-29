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
 * @file ConfigDataBase.hpp
 * @brief ConfigDataBase class header. This class is an interface for the ConfigData classes
 * @date 2024-02-07
 */

#ifndef YGGDRASILWM_CONFIGDATABASE_H
#define YGGDRASILWM_CONFIGDATABASE_H
#include "json/json.h"

/**
 * @class ConfigDataBase
 * @brief ConfigDataBase class
 * This class is an interface for the ConfigData classes
 * This is a pure virtual class and should be inherited by the ConfigData classes
*/
class ConfigDataBase {
public:
	ConfigDataBase() = default;
	virtual ~ConfigDataBase() = default;
/**
 * @fn virtual void ConfigDataBase::configInit(Json::Value& root_)
 * @brief Initialize the configuration
 * @param root_
 */
	virtual void configInit(const Json::Value& root_) = 0;
/**
 * @fn virtual Json::Value ConfigDataBase::configSave()
 * @brief Build a Json::Value object with the configuration
 * @return Json::Value Object with the current runtime configuration
 */
	virtual Json::Value configSave() = 0;
};

#endif //YGGDRASILWM_CONFIGDATABASE_H
