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
 * @file Grow.hpp
 * @brief Grow class header.
 * @date 2024-02-12
 */
#ifndef YGGDRASILWM_GROW_HPP
#define YGGDRASILWM_GROW_HPP
#include "Commands/CommandBase.hpp"
extern "C" {
#include <X11/Xlib.h>
}
/**
 * @class Grow
 * @brief Grow a window
 */
struct DeserializedXKeyEvent {
	std::string args;
	Window subwindow;
	Window window;
	unsigned int state;
	unsigned int keycode;
};

class Grow : public CommandBase {
public:
	Grow();
	~Grow() override = default;
/**
 * @fn void FocusGroup::execute(const std::string &args)
 * @brief switch from the current active group to the group index passed as argument
 * @param args the string will be parsed to an integer to switch to the group index
 */
	void	execute(const std::string &args) override;
	static DeserializedXKeyEvent deserializeXKeyEvent(const std::string &serializedEvent);
};
#endif //YGGDRASILWM_GROW_HPP