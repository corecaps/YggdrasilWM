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
 * @date 2024-02-12
 */
#ifndef YGGDRASILWM_BINDING_H
#define YGGDRASILWM_BINDING_H
#include <string>
#include "Commands/CommandBase.hpp"
extern "C" {
#include <X11/Xlib.h>
}
class BaseX11Wrapper;
/**
 * @class Binding
 * @brief Binding store a binding to a command object
 */
class Binding {
public:
			Binding();
			~Binding();
/**
 * @fn void Binding::init(std::string Mod, std::string Key, std::string Command, std::string Args)
 * @brief Initialize a binding
 * all the parameters are read from the Json::Value from the configuration file
 * Mod is parsed to store the proper modMask
 * Command is parsed to link the proper command object
 * @note you need to call init_keycode after the display is opened
 */
	void	init(std::string Mod, std::string Key, std::string Command, std::string Args);
/**
 * @fn void Binding::init_keycode(Display *display)
 * @brief Initialize the keycode of the binding
 * had to be initialized after the display is opened
 * @param display
 */
	void	init_keycode(Display *display, BaseX11Wrapper *x11Wrapper);
/**
 * @fn void Binding::execute()
 * @brief Execute the command linked to the binding
 * The arguments passed to the command are the one stored in the binding
 */
	void execute();
	[[nodiscard]] const std::string &	getMod() const;
	[[nodiscard]] const std::string &	getKey() const;
	[[nodiscard]] const std::string &	getCommandName() const;
	[[nodiscard]] const std::string &	getArgs() const;
	[[nodiscard]] unsigned int			getModMask() const;
	[[nodiscard]] int					getKeyCode() const;
private:
	std::string		mod_;
	unsigned int	modMask_;
	std::string		key_;
	int				keyCode_;
	std::string		commandName_;
	std::string		args_;
	CommandBase*	command_;
};
#endif //YGGDRASILWM_BINDING_H