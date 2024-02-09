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
 * @date 2024-02-08
 *
 */

#ifndef YGGDRASILWM_CONFIGHANDLER_HPP
#define YGGDRASILWM_CONFIGHANDLER_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "json/json.h"
#include "ConfigDataBase.hpp"
#include <variant>
#include <cctype>
#include <typeindex>
class ConfigData;

class ConfigFileHandler;
/**
 * @class ConfigHandler
 * @brief ConfigHandler class
 * This class is responsible for handling the configuration
 * It can be created with a file name or not, if not config
 * files will be looked up in default path defined in ConfigFileHandler
 * ConfigHandler is a Singleton class and should be created after
 * the Logger class and before the WindowManager class.
 * @see ConfigFileHandler
 * @see ConfigDataBase
 */
class ConfigHandler {
public:
	ConfigHandler(const ConfigHandler&) = delete;
	ConfigHandler& operator=(const ConfigHandler&) = delete;
/**
 * @fn template <typename T> T* ConfigHandler::getConfigData()
 * @brief Get the ConfigData object of type T
 * @tparam T Type of the ConfigData object
 * @return a pointer to an object form a subclass of ConfigData
 */
	template <typename T>
	T* getConfigData() {
		auto it = configMap_.find(std::type_index(typeid(T)));
		if (it == configMap_.end()) {
			throw std::runtime_error("ConfigData not found");
		}
		return static_cast<T*>(it->second);
	}
/**
 * @fn template <typename T> void ConfigHandler::addConfigData(T* configData)
 * @tparam T Type of the ConfigData object you want to add to the ConfigHandler
 * @param configData the ConfigData object you want to add
 */
	template <typename T>
	void addConfigData(T* configData) {
		static_assert(std::is_base_of<ConfigDataBase, T>::value, "T must be a subclass of ConfigData");
		configMap_[std::type_index(typeid(T))] = configData;
	}
/**
 * @fn static void ConfigHandler::Create(const std::string& configPath)
 * @brief Create a ConfigHandler object with a path
 * @param configPath path to the configuration file
 */
	static void Create(const std::string& configPath);
/**
 * @fn static void ConfigHandler::Create()
 * @brief Create a ConfigHandler object without a path
 */
	static void Create();
/**
 * @fn static ConfigHandler& ConfigHandler::GetInstance()
 * @brief Get the ConfigHandler object Throws an exception if the object is not created
 */
	static ConfigHandler& GetInstance();
/**
 * @fn static void ConfigHandler::Destroy()
 * @brief Destroy the ConfigHandler object
 */
	static void Destroy();
/**
 * @fn Json::Value& ConfigHandler::getRoot()
 * @brief Get the root JSON object, the ConfigHandler::configInit() must be called first
 * @return the root JSON object in Json::Value
 */
	Json::Value& getRoot();
/**
 * @fn void ConfigHandler::configInit()
 * @brief Initialize the ConfigHandler
 * Read the configuration file and store the JSon root object in Json::Value root_
 * Get the configFilePath form which the config has been read
 */
	void configInit();
	~ConfigHandler();
/**
 * @fn unsigned long ConfigHandler::colorCodeToULong(const std::string &colorCode)
 * @brief Convert a string containing a color code to unsigned long
 * @param colorCode
 * @return unsigned long color code
 */
	static unsigned long colorCodeToULong(const std::string &colorCode);

private:
	std::string configPath_;
	std::unique_ptr<ConfigFileHandler> configFileHandler_;
	static ConfigHandler* instance_;
	Json::Value root_;
	std::unordered_map<std::type_index, ConfigDataBase * > configMap_;
/**
 * @fn ConfigHandler()
 * @brief Construct a new ConfigHandler object without a path
 * Instanciate a ConfigFileHandler Object without a path
 */
	ConfigHandler();
/**
 * @fn ConfigHandler(const std::string configPath)
 * @brief Construct a new ConfigHandler object with a path
 * Instanciate a ConfigFileHandler Object with a path
 * @param configPath
 */
	explicit ConfigHandler(const std::string& configPath);
};
#endif //YGGDRASILWM_CONFIGHANDLER_HPP
