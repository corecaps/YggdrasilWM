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
 * @date 2024-02-05
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
/**
 * @class ConfigHandler
 * @brief ConfigHandler class
 * This class is responsible for handling the configuration file.
 * It is used to read and write the configuration file. And
 * Transmits the configuration to the WindowManager class.
 * The constructor will search for the configuration file in the default paths.
 * @see WindowManager
 */
class ConfigHandler {
public:
	using ConfigValue = std::variant<std::string, int, bool,unsigned long>;
/**
 * @fn ConfigHandler()
 * @brief Construct a new ConfigHandler object without a path
 */
	ConfigHandler();
/**
 * @fn ConfigHandler(const std::string configPath)
 * @brief Construct a new ConfigHandler object with a path, if the path is not valid try to find the file in the default paths
 * @param configPath
 */
	explicit ConfigHandler(const std::string& configPath);
	~ConfigHandler();
/**
 * @fn void ConfigHandler::setConfig(const std::string &key, const std::string &value)
 * @brief Set a configuration value during runtime
 * @param key
 * @param value
 */
	void setConfig(const std::string &key, const std::string &value);
/**
 * @fn ConfigValue ConfigHandler::getConfig(const std::string &key)
 * @brief Get a configuration value
 * @param key
 * @return a variant of string, int, bool or unsigned long
 */
	ConfigValue getConfig(const std::string &key);

	void saveConfig();
/**
 * @fn bool ConfigHandler::loadConfig()
 * @brief Load the configuration file and parse it into a map
 * @return true if the file was loaded successfully false otherwise
 * @todo : Implement the saveConfig function
 */
	bool loadConfig();
/**
 * @fn const std::string &ConfigHandler::getConfigPath() const
 * @brief Get the Config Path object
 * @return a string with the path to the configuration file
 */
	const std::string &getConfigPath() const;
/**
 * @fn std::stringstream ConfigHandler::printConfig()
 * @brief Print the configuration to a string stream, this method is only used for debugging purpose and will be removed
 * @return
 */
	std::stringstream printConfig();
private:
	static const std::vector<std::string> defaultPaths;
	std::string configPath_;
	std::unordered_map<std::string,ConfigValue> configMap_;
/**
 * @fn std::string ConfigHandler::findConfigFile() const
 * @brief Find the configuration file in the default paths
 * @return a string with the path to the configuration file
 */
	static std::string findConfigFile() ;
/**
 * @fn bool ConfigHandler::fileExists(const std::string &path) const
 * @brief Check if a file exists
 * @param path
 * @return true if the file exists false otherwise
 */
	static bool fileExists(const std::string &path) ;
/**
 * @fn std::string ConfigHandler::expandEnvironmentVariables(const std::string &path) const
 * @brief Expand environment variables in a string
 * @param path
 * @return expanded string
 */
	static std::string expandEnvironmentVariables(const std::string &path) ;
/**
 * @fn std::unordered_map<std::string, ConfigValue> ConfigHandler::parseJsonToMap(const Json::Value &jsonValue)
 * @brief Parse a json value to a map
 * @param jsonValue
 * @return a map with the json values
 */
	std::unordered_map<std::string, ConfigValue> parseJsonToMap(const Json::Value& jsonValue);
/**
 * @fn unsigned long ConfigHandler::colorCodeToULong(const std::string &colorCode)
 * @brief Convert a string containing a color code to unsigned long
 * @param colorCode
 * @return unsigned long color code
 */
	static unsigned long colorCodeToULong(const std::string &colorCode);
};
#endif //YGGDRASILWM_CONFIGHANDLER_HPP
