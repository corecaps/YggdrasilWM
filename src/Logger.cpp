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
 * @file Logger.cpp
 * @brief Logger class implementation.
 * @date 2024-02-04
 *
 */
#include "Logger.hpp"
Logger::Logger(const std::string& logFile, LogLevel logLevel)
		: logLevel_(logLevel) {
	auto* fileStream = new std::ofstream(logFile, std::ios::out | std::ios::app);
	if (!fileStream->good()) {
		std::cerr << "Failed to open log file: " << logFile << std::endl;
		delete fileStream;
		throw std::runtime_error("Failed to open log file.");
	}
	logStream_ = fileStream;
	streamIsFile_ = true;
}
Logger::Logger(std::ostream& output, LogLevel logLevel)
		: logStream_(&output), logLevel_(logLevel) {
	streamIsFile_ = false;
}
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

void Logger::Log(const std::string& message, LogLevel level) const {
	if (level < logLevel_) {
		return;
	}
	*logStream_ << GetTime() << GetLogLevel(level) << message << std::endl;
}
std::string Logger::GetTime() {
	time_t now = time(nullptr);
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