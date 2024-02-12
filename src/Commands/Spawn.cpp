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
 * @file Spawn.cpp
 * @brief Spawn class implementation.
 * @date 2024-02-12
 */

#include "Commands/Spawn.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <stdexcept>
#include <cstring>
#include <vector>
#include "Logger.hpp"

Spawn::Spawn() = default;
void Spawn::execute(const std::string &args) {
	std::istringstream iss(args);
	std::string command;
	iss >> command;
	std::vector<const char*> argv;
	argv.push_back(command.c_str());
	std::string arg;
	while (iss >> arg) {
		argv.push_back(arg.c_str());
	}
	argv.push_back(nullptr);
	pid_t pid = fork();
	if (pid < 0) {
		throw std::runtime_error("Fork failed: " + std::string(strerror(errno)));
	} else if (pid == 0) {
		pid_t pid_inner = fork();
		if (pid_inner < 0) {
			exit(EXIT_FAILURE);
		} else if (pid_inner == 0) {
			execvp(command.c_str(), const_cast<char* const*>(argv.data()));
			Logger::GetInstance()->Log("Failed to execute command \""
										+ command
										+ "\": "
										+ strerror(errno),L_ERROR);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	} else {
		waitpid(pid, nullptr, 0);
	}
	Logger::GetInstance()->Log("Succefully launched " + args,L_INFO);
}