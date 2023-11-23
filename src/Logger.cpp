//
// Created by corecaps on 11/21/23.
//

#include "Logger.hpp"
#include <utility>

Logger::Logger(const std::string& logFile, LogLevel logLevel)
		: logLevel_(logLevel) {
	logStream_ = std::make_unique<std::ofstream>(logFile, std::ios::out | std::ios::app);
	if (!logStream_->good()) {
		std::cerr << "Failed to open log file: " << logFile << std::endl;
		throw std::runtime_error("Failed to open log file.");
	}
}
Logger::Logger(std::ostream& output, LogLevel logLevel)
		: logStream_(&output), logLevel_(logLevel) {}

Logger::~Logger() {
	*logStream_ << GetTime() << "Closing Session \n"
		<< " =================================================================================== "
		<< std::endl;
	if (dynamic_cast<std::ofstream*>(logStream_.get()) != nullptr) {
		dynamic_cast<std::ofstream*>(logStream_.get())->close();
	}
}

void Logger::Log(const std::string& message, LogLevel level) const {
	if (level < logLevel_) {
		return;
	}
	*logStream_ << GetTime() << GetLogLevel(level) << message << std::endl;
}

std::string Logger::GetTime() {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	std::stringstream ss;
	ss << std::put_time(ltm, "[%Y-%m-%d-%H:%M:%S]\t");
	return ss.str();
}

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