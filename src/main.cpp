/**
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
			("loglevel", "Specify the log level (0-2)", cxxopts::value<int>());
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
		if (!(logLevel >= 0 && logLevel <= 2)) {
			std::cerr << "Invalid log level. It should be between 0 and 2." << std::endl;
			return EXIT_FAILURE;
		}
	}
	Logger logger(LogFilePath, static_cast<LogLevel>(logLevel));
	unique_ptr<WindowManager> window_manager(WindowManager::Create(logger));
	if (!window_manager) {
		logger.Log("Failed to initialize WindowManager.", L_ERROR);
		return EXIT_FAILURE;
	}
	window_manager->Run();
	return (EXIT_SUCCESS);
}
