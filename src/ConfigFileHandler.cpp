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
 * @date 2024-02-06
 *
 */
#include "ConfigFileHandler.hpp"
#include "Logger.hpp"
const std::vector<std::string> ConfigFileHandler::defaultPaths = {
		"config.json",
		"$HOME/.config/YggdrasilWM/config.json",
		"$HOME/.YggdrasilWM/config.json",
		"/etc/YggdrasilWM/config.json"
};
ConfigFileHandler::ConfigFileHandler() : configPath_(""),root_() {
	this->configPath_ = findConfigFile();
	if (this->configPath_.empty()) {
		throw std::runtime_error("No config file found.");
	}
	Logger::GetInstance()->Log("Config file found at: " + this->configPath_ ,L_INFO);
}
ConfigFileHandler::ConfigFileHandler(const std::string& configPath) : configPath_("") {
	if (fileExists(configPath)) {
		this->configPath_ = configPath;
	} else {
		this->configPath_ = findConfigFile();
	}
	if (this->configPath_.empty()) {
		throw std::runtime_error("No config file found.");
	}
	Logger::GetInstance()->Log("Using Config file : " + this->configPath_,L_INFO );
}
ConfigFileHandler::~ConfigFileHandler() = default;
bool ConfigFileHandler::fileExists(const std::string& path) {
	std::ifstream f(path.c_str());
	return f.good();
}
std::string ConfigFileHandler::findConfigFile() {
	for (const auto&path : defaultPaths) {
		if (fileExists(expandEnvironmentVariables(path))) {
			return path;
		}
	}
	return "";
}
std::string ConfigFileHandler::expandEnvironmentVariables(const std::string& path) {
	if (path.empty() || path[0] != '$') {
		return path;
	}
	std::string variable = path.substr(1);
	const char* value = std::getenv(variable.c_str());
	return value ? value : "";
}
void ConfigFileHandler::readConfig() {
	std::ifstream file(configPath_);
	if (!file.is_open())
		throw std::runtime_error("Failed to open config file");
	try {
		Json::CharReaderBuilder readerBuilder;
		Json::parseFromStream(readerBuilder, file, &root_, nullptr);
		file.close();
	} catch (Json::Exception &e) {
		Logger::GetInstance()->Log("Failed to parse config file: " + std::string(e.what()),L_ERROR);
	}
}
void ConfigFileHandler::writeConfig(const Json::Value& root) {
	std::ofstream file(configPath_);
	if (!file.is_open())
		throw std::runtime_error("Failed to open config file");
	try {
		Json::StreamWriterBuilder writer;
		writer["indentation"] = "\t";
		std::string output = Json::writeString(writer, root);
		file << output;
		file.close();
	} catch (Json::Exception &e) {
		Logger::GetInstance()->Log("Failed to write config file: " + std::string(e.what()),L_ERROR);
	}
}
Json::Value ConfigFileHandler::getRoot() { return root_; }

std::string ConfigFileHandler::getConfigPath() {
	return configPath_;
}
