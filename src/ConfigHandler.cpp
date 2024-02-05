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
 * @file ConfigHandler.cpp
 * @brief ConfigHandler class implementation.
 * @date 2024-02-05
 *
 */

#include "ConfigHandler.hpp"
using ConfigValue = std::variant<std::string, int, bool,unsigned long>;

const std::vector<std::string> ConfigHandler::defaultPaths = {
		"config.json",
		"$HOME/.config/YggdrasilWM/config.json",
		"$HOME/.YggdrasilWM/config.json",
		"/etc/YggdrasilWM/config.json"
};
ConfigHandler::ConfigHandler() : configPath_(""), configMap_() {
	this->configPath_ = findConfigFile();
	if (this->configPath_.empty()) {
		throw std::runtime_error("No config file found.");
	}
	std::cout << "Config file found at: " << this->configPath_ << std::endl;
}
ConfigHandler::ConfigHandler(const std::string& configPath) : configPath_(""), configMap_() {
	if (fileExists(configPath)) {
		this->configPath_ = configPath;
	} else {
		this->configPath_ = findConfigFile();
	}
	if (this->configPath_.empty()) {
		throw std::runtime_error("No config file found.");
	}
	std::cout << "Config file found at: " << this->configPath_ << std::endl;
}
ConfigHandler::~ConfigHandler() = default;
void ConfigHandler::setConfig(const std::string &key, const std::string &value) {

}
ConfigValue ConfigHandler::getConfig(const std::string &key) {
	auto it = configMap_.find(key);
	if (it != configMap_.end()) {
		return it->second;
	}
	return ConfigValue {};
}
void ConfigHandler::saveConfig() {
}
bool ConfigHandler::fileExists(const std::string& path) {
	std::ifstream f(path.c_str());
	return f.good();
}
std::string ConfigHandler::findConfigFile() {
	for (const auto&path : defaultPaths) {
		if (fileExists(expandEnvironmentVariables(path))) {
			return path;
		}
	}
	return "";
}
std::string ConfigHandler::expandEnvironmentVariables(const std::string& path) {
	if (path.empty() || path[0] != '$') {
		return path;
	}
	std::string variable = path.substr(1);
	const char* value = std::getenv(variable.c_str());
	return value ? value : "";
}
bool ConfigHandler::loadConfig() {
	std::ifstream file(configPath_);
	if (!file.is_open()) {
		return false;
	}
	try {
		Json::CharReaderBuilder readerBuilder;
		Json::Value root;
		Json::parseFromStream(readerBuilder, file, &root, nullptr);
		configMap_ = parseJsonToMap(root);
		return true;
	} catch (const Json::Exception &e) {
		std::cerr << "Error parsing config file: " << e.what() << std::endl;
		return false;
	}
}
std::unordered_map<std::string, ConfigValue> ConfigHandler::parseJsonToMap(const Json::Value& jsonValue) {
	std::unordered_map<std::string, ConfigValue> result;
	for (const auto& key : jsonValue.getMemberNames()) {
		const Json::Value& jsonSubValue = jsonValue[key];
		if (jsonSubValue.isString()) {
			if (jsonSubValue.asString().size() == 7 && jsonSubValue.asString()[0] == '#') {
				result[key] = colorCodeToULong(jsonSubValue.asString());
			} else {
				result[key] = jsonSubValue.asString();
			}
		} else if (jsonSubValue.isInt()) {
			result[key] = jsonSubValue.asInt();
		} else if (jsonSubValue.isBool()) {
			result[key] = jsonSubValue.asBool();
		} else {
			std::cerr << "Unsupported value type for key: " << key << std::endl;
		}
	}
	return result;
}
std::stringstream ConfigHandler::printConfig() {
	std::stringstream result;
	result << "\n\tConfig file: " << configPath_ << std::endl;
	for (const auto& keyValue : configMap_) {
		const auto& key = keyValue.first;
		const ConfigValue& value = keyValue.second;
		std::visit([&result, &key](const auto& v) {
			result << "\t\t" << key << " : " << v << std::endl;
		}, value);
	}
	return result;
}
const std::string &ConfigHandler::getConfigPath() const { return configPath_; }
unsigned long ConfigHandler::colorCodeToULong(const std::string& colorCode) {
	if (colorCode.size() != 7 || colorCode[0] != '#' || !isxdigit(colorCode[1])) {
		return 0x000000;
	}
	std::istringstream iss(colorCode.substr(1));
	unsigned long colorValue;
	iss >> std::hex >> colorValue;
	if (iss.fail()) { return 0x000000; }
	return colorValue;
}