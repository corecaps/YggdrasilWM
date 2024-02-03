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
/**
 * @brief Log levels
 * L_INFO: Informational messages
 * L_WARNING: Warning messages
 * L_ERROR: Error messages
 */
enum LogLevel {
	L_INFO,
	L_WARNING,
	L_ERROR,
};
/**
 * @brief Logger class
 * This class is responsible for logging.
 * It can be created with a file name or an ostream.
 * The log level can be set to filter the messages.
 */
class Logger {
public:
/**
 * @brief Construct a new Logger:: Logger object
 * This constructor is used when the user wants to log to a file.
 * @param logFile the file to log to
 * @param logLevel level of logging 0: info, 1: warning, 2: error
 */
	Logger(const std::string& logFile, LogLevel logLevel);
/**
 * @brief Construct a new Logger:: Logger object
 * This constructor is used when the user wants to log to a stream.
 * @param output the stream to log to
 * @param logLevel level of logging 0: info, 1: warning, 2: error
 */
	Logger(std::ostream& output, LogLevel logLevel);
/**
 * @brief Destroy the Logger:: Logger object
 * closes the log file if it was opened.
 * The destructor is virtual to allow for subclassing.for gmock
 */
	virtual ~Logger();
/**
 * @brief Log a message
 * This method logs a message to the log file or stream.
 * The message is only logged if the log level is high enough.
 * the time and log level are prepended to the message.
 * The method is virtual to allow for subclassing.for gmock
 * @param message the message to log
 * @param level the level of the message
 */
	virtual void Log(const std::string& message, LogLevel level) const;

private:
/**
 * @brief Get the log level as a string
 * @param level
 * @return a string representation of the log level
 */
	static std::string GetLogLevel(LogLevel level);
/**
 * @brief Get the current time
 *  * This method returns the current time in the format: [YYYY-MM-DD-HH:MM:SS]
 * @return
 */
	static std::string GetTime();
/**
 * @brief check if the stream is a file or a stream
 * @return true if the stream is a file
 */
	bool streamIsFile_;
	std::ostream* logStream_;
	LogLevel logLevel_;
};
#endif //WINDOWMANAGER_LOGGER_H
