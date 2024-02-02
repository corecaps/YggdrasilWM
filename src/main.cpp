/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
 * @file main.cpp
 * @brief Main file.
 * @date 2021-06-23
 * YggdrasilWM is a tiny window manager for X11.
 */

#include <cstdlib>
#include "window_manager.hpp"
#include "Logger.hpp"
#include <cxxopts.hpp>

using ::std::unique_ptr;
/**
 * @brief YggdrasilWM
 * Options are :
 * --version
 * --log /path/to/log/file
 * --loglevel 0-2.
 */

int main(int argc, char** argv) {
	cxxopts::Options options(PROGRAM_NAME, "A tiny Window Manager.");
	options.add_options()
			("version", "Display this text and exit", cxxopts::value<bool>())
			("log", "Specify the log file path", cxxopts::value<std::string>())
			("loglevel", "Specify the log level (0-2)", cxxopts::value<int>())
			("d,display", "Specify the display to use", cxxopts::value<std::string>());
	auto result = options.parse(argc, argv);

	if (result["version"].as<bool>()) {
		std::cout << PROGRAM_NAME << "\t" << PROGRAM_VERSION << "\n" << std::endl;
		std::cout << options.help() << std::endl;
		return EXIT_SUCCESS;
	}
	std::string LogFilePath;
	if (result.count("log")) {
		LogFilePath = result["log"].as<std::string>();
	} else {
		LogFilePath = "wm.log";
	}
	int logLevel = 0;
	if (result.count("loglevel")) {
		logLevel = result["loglevel"].as<int>();
		if (logLevel < 0 || logLevel > 2) {
			std::cerr << "Invalid log level. It should be between 0 and 2." << std::endl;
			return EXIT_FAILURE;
		}
	}
//	Logger logger(LogFilePath, static_cast<LogLevel>(logLevel));
	Logger logger(std::cout, static_cast<LogLevel>(logLevel));
	logger.Log("Starting " + std::string(PROGRAM_NAME) + " " + std::string(PROGRAM_VERSION), L_INFO);
	unique_ptr<WindowManager> windowManager;
	if (result.count("display")) {
		logger.Log("Using display " + result["display"].as<std::string>(), L_INFO);
		windowManager = WindowManager::Create(logger, result["display"].as<std::string>());
	} else {
		logger.Log("Using default display", L_INFO);
		windowManager = WindowManager::Create(logger);
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
