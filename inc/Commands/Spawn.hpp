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
 * @file Spawn.hpp
 * @brief Spawn class header.
 * @date 2024-02-12
 */
#ifndef YGGDRASILWM_SPAWN_HPP
#define YGGDRASILWM_SPAWN_HPP
#include "Commands/CommandBase.hpp"
/**
 * @class Spawn
 * @brief Implement the mecanism to spawn a program
 */
class Spawn : public CommandBase {
public:
			Spawn();
			~Spawn() override = default;
/**
 * @fn void Spawn::execute(const std::string &args)
 * @brief Spawn a program using the command arguments
 * Double fork to avoid zombie processes
 * Parse the command arguments to get the program name and its arguments
 * use of execvp to execute the command so it will search the bin using PATH
 * throw an exception if the command fails
 * @param args the first word separated by a space is the binary name, the rest is the arguments
 */
	void	execute(const std::string &args) override;
};
#endif //YGGDRASILWM_SPAWN_HPP