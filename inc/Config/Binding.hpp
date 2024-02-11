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
 * @file Binding.hpp
 * @brief Binding class header.
 * @date 2024-02-11
 */
#ifndef YGGDRASILWM_BINDING_H
#define YGGDRASILWM_BINDING_H
#include <string>
#include "Commands/CommandBase.hpp"

class Binding {
public:
	Binding();
	~Binding() = default;
	void init(std::string Mod, std::string Key, std::string Command, std::string Args);
	void execute();
	const std::string &getMod() const;
	const std::string &getKey() const;
	const std::string &getCommandName() const;
	const std::string &getArgs() const;

private:
public:
	unsigned int getModMask() const;

	int getKeyCode() const;

private:
	std::string mod_;
	unsigned int modMask_;
	std::string key_;
	int keyCode_;
	std::string commandName_;
	std::string args_;
	CommandBase *command_;
};
#endif //YGGDRASILWM_BINDING_H