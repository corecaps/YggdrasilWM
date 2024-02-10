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
 * @file Binding.cpp
 * @brief Binding class implementation.
 * @date 2024-02-11
 */
#include <string>
#include "Config/Binding.hpp"
void Binding::init(std::string Mod, std::string Key, std::string Command, std::string Args) {
	mod_ = Mod;
	key_ = Key;
	commandName_ = Command;
	args_ = Args;
}
void Binding::execute() {
	command_->execute(args_);
}
Binding::Binding() {

}
const std::string &Binding::getMod() const { return mod_; }
const std::string &Binding::getKey() const { return key_; }
const std::string &Binding::getCommandName() const { return commandName_; }
const std::string &Binding::getArgs() const { return args_; }