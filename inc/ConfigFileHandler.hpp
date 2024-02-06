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
 * @file ConfigFileHandler.cpp
 * @brief ConfigFileHandler class implementation.
 * @date 2024-02-05
 *
 */
#ifndef YGGDRASILWM_CONFIGFILEHANDLER_H
#define YGGDRASILWM_CONFIGFILEHANDLER_H
#include <string>
#include <fstream>
#include <iostream>
#include <json/json.h>
/**
 * @class ConfigFileHandler
 * @brief Handle file I/O and JSON parsing for the configuration file.
 * If no path is provided, it will search for the configuration file
 * in the default paths.
 */
class ConfigFileHandler {
public:
/**
 * @fn ConfigFileHandler::ConfigFileHandler()
 * @brief Construct a new ConfigFileHandler object without a path
 */
	ConfigFileHandler();
/**
 * @fn ConfigFileHandler::ConfigFileHandler(const std::string configPath)
 * @param configPath Full path to the configuration file
 * @brief Construct a new ConfigFileHandler object with a path,
 * if the path is not valid try to find the file in the default paths
 */
	ConfigFileHandler(const std::string& configPath);
	~ConfigFileHandler();
/**
 * @fn void ConfigFileHandler::readConfig()
 * @brief Read the configuration file
 * Store the JSon root object in Json::Value root_
 * Throws exceptions in case of file i/o error
 * Catch Parsing Errors and log them with Logger Object
 */
	void readConfig();
/**
 * @fn void ConfigFileHandler::writeConfig(const Json::Value& config)
 * @brief Write the configuration file
 * Store the JSon root object in the configPath file
 * Throws exceptions in case of file i/o error
 * @param config The root JSON object to write
 */
	void writeConfig(const Json::Value& root);
/**
 * @fn Json::Value ConfigFileHandler::getRoot()
 * @brief Get the root JSON object
 * @return Json::Value The root JSON object
 */
	Json::Value getRoot();
	std::string getConfigPath();
private:
	static const std::vector<std::string> defaultPaths;
	std::string configPath_;
	Json::Value root_;
/**
 * @fn std::string ConfigFileHandler::findConfigFile()
 * @brief Find the configuration file in the default paths
 * @return The full path to the configuration file
 */
	static std::string findConfigFile();
/**
 * @fn bool ConfigFileHandler::fileExists(const std::string& path)
 * @brief Check if a file exists by trying to open it
 * @param path
 * @return
 */
	static bool fileExists(const std::string &path);
/**
 * @fn std::string ConfigFileHandler::expandEnvironmentVariables(const std::string& path)
 * @brief Expand environment variables in a path
 * @param path
 * @return Expanded path as a std::string
 */
	static std::string expandEnvironmentVariables(const std::string &path);
};

#endif //YGGDRASILWM_CONFIGFILEHANDLER_H
