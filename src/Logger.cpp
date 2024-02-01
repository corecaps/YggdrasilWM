/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
 * @file Logger.cpp
 * @brief Logger class implementation.
 * @date 2021-06-23
 *
 */
#include "Logger.hpp"
#include <utility>

/**
 * @brief Construct a new Logger:: Logger object
 * This constructor is used when the user wants to log to a file.
 * @param logFile the file to log to
 * @param logLevel level of logging 0: info, 1: warning, 2: error
 */

Logger::Logger(const std::string& logFile, LogLevel logLevel)
		: logLevel_(logLevel) {
	std::ofstream* fileStream = new std::ofstream(logFile, std::ios::out | std::ios::app);
	if (!fileStream->good()) {
		std::cerr << "Failed to open log file: " << logFile << std::endl;
		delete fileStream;
		throw std::runtime_error("Failed to open log file.");
	}
	logStream_ = fileStream;
	streamIsFile_ = true;
}
/**
 * @brief Construct a new Logger:: Logger object
 * This constructor is used when the user wants to log to a stream.
 * @param output the stream to log to
 * @param logLevel level of logging 0: info, 1: warning, 2: error
 */

Logger::Logger(std::ostream& output, LogLevel logLevel)
		: logStream_(&output), logLevel_(logLevel) {
	streamIsFile_ = false;
}
/**
 * @brief Destroy the Logger:: Logger object
 * closes the log file if it was opened.
 */

Logger::~Logger() {
	*logStream_ << GetTime() << "Closing Session \n"
				<< " =================================================================================== "
				<< std::endl;
	if (streamIsFile_) {
		if (dynamic_cast<std::ofstream*>(logStream_) != nullptr) {
			dynamic_cast<std::ofstream*>(logStream_)->close();
		}
		delete logStream_;
	}
}
/**
 * @brief Log a message
 * This method logs a message to the log file or stream.
 * The message is only logged if the log level is high enough.
 * the time and log level are prepended to the message.
 * @param message the message to log
 * @param level the level of the message
 */

void Logger::Log(const std::string& message, LogLevel level) const {
	if (level < logLevel_) {
		return;
	}
	*logStream_ << GetTime() << GetLogLevel(level) << message << std::endl;
}

/**
 * @brief Get the current time
 * This method returns the current time in the format: [YYYY-MM-DD-HH:MM:SS]
 * @return std::string the current time
 */

std::string Logger::GetTime() {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	std::stringstream ss;
	ss << std::put_time(ltm, "[%Y-%m-%d-%H:%M:%S]\t");
	return ss.str();
}
/**
 * @brief Get the log level
 * This method returns the log level in the format: [LOGLEVEL]
 * @param level the log level
 * @return std::string the log level
 */

std::string Logger::GetLogLevel(LogLevel level) {
	switch (level) {
		case L_INFO:
			return "[INFO]\t";
		case L_WARNING:
			return "[WARNING]\t";
		case L_ERROR:
			return "[ERROR]\t";
		default:
			return "[UNKNOWN]\t";
	}
}