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
 * @file main.cpp
 * @brief Main file.
 * @date 2024-02-04
 * YggdrasilWM is a tiny window manager for X11.
 */

#include <cstdlib>
#include "window_manager.hpp"
#include "Logger.hpp"
#include <cxxopts.hpp>
#include "ConfigHandler.hpp"

/**
 * @fn int main(int argc, char** argv)
 * @brief YggdrasilWM
 * Uses cxxopts to parse command line arguments and starts the WindowManager.
 * creates a Logger and a ConfigHandler.
 */

int main(int argc, char** argv) {
	std::cout << "    YggdrasilWM  Copyright (C) 2024  corecaps <corecaps@gmail.com> <jgarcia@student.42.fr>\n"
				 "    This program comes with ABSOLUTELY NO WARRANTY.\n"
				 "    This is free software, and you are welcome to redistribute it\n"
				 "    under certain conditions." << std::endl;
	cxxopts::Options options(PROGRAM_NAME, "YggdrasilWM by corecaps. https://github.com/corecaps/YggdrasilWM");
	options.add_options()
			("h,help", "Print help")
			("v,version", "Display version", cxxopts::value<bool>())
			("l,log", "Specify the log file path", cxxopts::value<std::string>())
			("loglevel", "Specify the log level (0-2)", cxxopts::value<int>())
			("c,config", "Specify the config file path", cxxopts::value<std::string>())
			("d,display", "Specify the display to use", cxxopts::value<std::string>());
	std::string LogFilePath;
	std::string Display;
	std::string ConfigFilePath;
	int logLevel = 0;
	try {
		auto result = options.parse(argc, argv);
		if (result.count("help")) {
			std::cout << options.help() << std::endl;
			return EXIT_SUCCESS;
		}
		if (result["version"].as<bool>()) {
			std::cout << PROGRAM_NAME << "\t" << PROGRAM_VERSION << "\n" << std::endl;
			return EXIT_SUCCESS;
		}
		if (result.count("log")) {
			LogFilePath = result["log"].as<std::string>();
		} else {
			LogFilePath = "wm.log";
		}
		if (result.count("loglevel")) {
			logLevel = result["loglevel"].as<int>();
			if (logLevel < 0 || logLevel > 2) {
				std::cerr << "Invalid log level. It should be between 0 and 2." << std::endl;
				return EXIT_FAILURE;
			}
		}
		if (result.count("config")) {
			ConfigFilePath = result["config"].as<std::string>();
		} else {
			ConfigFilePath = "";
		}
		if (result.count("display")) {
			Display = result["display"].as<std::string>();
		} else {
			Display = "";
		}
	} catch (const cxxopts::OptionException &e) {
		std::cerr << "Error parsing options: " << e.what() << std::endl;
		std::cout << options.help() << std::endl;
		return EXIT_FAILURE;
	}
	//	Logger logger(LogFilePath, static_cast<LogLevel>(logLevel));
	Logger logger(std::cout, static_cast<LogLevel>(logLevel));
	ConfigHandler configHandler = ConfigHandler(ConfigFilePath);
	bool validConfig = configHandler.loadConfig();
	if (validConfig)
		logger.Log("Loaded config: " + configHandler.getConfigPath(), L_INFO);
	else {
		logger.Log("Failed to load config: " + configHandler.getConfigPath(),L_ERROR);
		return EXIT_FAILURE;
	}
	logger.Log(configHandler.printConfig().str(), L_INFO);
	std::cout << std::endl;
	logger.Log("Starting " + std::string(PROGRAM_NAME) + " " + std::string(PROGRAM_VERSION), L_INFO);
	std::unique_ptr<WindowManager> windowManager;
	if (!Display.empty()) {
		logger.Log("Using display " + Display, L_INFO);
		windowManager = WindowManager::Create(logger,configHandler, Display);
	} else {
		logger.Log("Using default display", L_INFO);
		windowManager = WindowManager::Create(logger, configHandler);
	}
	if (!windowManager) {
		logger.Log("Failed to initialize WindowManager.", L_ERROR);
		return EXIT_FAILURE;
	}
	logger.Log("Starting WindowManager.", L_INFO);
	try {
		windowManager->Init();
	} catch (const std::exception &e) {
		logger.Log(e.what(), L_ERROR);
		return EXIT_FAILURE;
	}
	windowManager->Run();
	return (EXIT_SUCCESS);
}
