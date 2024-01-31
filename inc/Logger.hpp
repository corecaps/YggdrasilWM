/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
 * @file Logger.hpp
 * @brief Logger class header.
 * @date 2021-06-23
 * This class is responsible for logging.
 * It can be created with a file name or an ostream.
 * The log level can be set to filter the messages.
 */

#ifndef WINDOWMANAGER_LOGGER_H
#define WINDOWMANAGER_LOGGER_H
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <memory>

enum LogLevel {
	L_INFO,
	L_WARNING,
	L_ERROR,
};

class Logger {
public:
	Logger(const std::string& logFile, LogLevel logLevel);
	Logger(std::ostream& output, LogLevel logLevel);
	~Logger();
	void Log(const std::string& message, LogLevel level) const;

private:
	static std::string GetLogLevel(LogLevel level);
	static std::string GetTime();

	std::unique_ptr<std::ostream> logStream_;
	LogLevel logLevel_;
};
#endif //WINDOWMANAGER_LOGGER_H
