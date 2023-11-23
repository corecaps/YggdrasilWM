//
// Created by corecaps on 11/21/23.
//

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
