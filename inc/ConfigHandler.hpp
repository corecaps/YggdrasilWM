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
 * @file ConfigHandler.hpp
 * @brief ConfigHandler class header.
 * @date 2021-06-23
 *
 */

#ifndef YGGDRASILWM_CONFIGHANDLER_HPP
#define YGGDRASILWM_CONFIGHANDLER_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <json/json.h>
#include <variant>
#include <cctype>

class ConfigHandler {
public:
	using ConfigValue = std::variant<std::string, int, bool,unsigned long>;

	ConfigHandler();
	ConfigHandler(const std::string configPath);
	~ConfigHandler();
	void setConfig(const std::string &key, const std::string &value);
	ConfigValue getConfig(const std::string &key);
	void saveConfig();
	bool loadConfig();

	const std::string &getConfigPath() const;

	std::stringstream printConfig();
private:
	static const std::vector<std::string> defaultPaths;
	std::string configPath_;
	std::unordered_map<std::string,ConfigValue> configMap_;
	std::string findConfigFile() const;
	bool fileExists(const std::string &path) const;
	std::string expandEnvironmentVariables(const std::string &path) const;
	std::unordered_map<std::string, ConfigValue> parseJsonToMap(const Json::Value& jsonValue);

	unsigned long colorCodeToULong(const std::string &colorCode);
};



#endif //YGGDRASILWM_CONFIGHANDLER_HPP
