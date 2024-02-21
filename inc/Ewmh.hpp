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
 * @file Ewmh.hpp
 * @brief ewmh namespace header.
 * @date 2024-02-11
 */

#ifndef WINDOW_MANAGER_EWMH_HPP
#define WINDOW_MANAGER_EWMH_HPP
extern "C" {
#include <X11/Xlib.h>
#include <X11/Xatom.h>
};
#include <string>
/**
 * @namespace ewmh
 * @brief ewmh namespace
 * This namespace contains the EWMH related functions.
 */
namespace ewmh {
/**
 * @fn void initEwmh(Display *display, Window root)
 * @brief register the supported EWMH atoms.
 * @param display must be opened before call to this function.
 * @param root root window the wm is managing (usually the default root window)
 */
	void	initEwmh(Display *display, Window root);
	void	handleMessage(XClientMessageEvent *event, Display *display, Window root);
	void	updateNumberOfDesktops(Display *display, Window root);
	void	updateWmProperties(Display *display, Window root);
	void	updateDesktopGeometry(Display *display, Window root);
	void	updateActiveWindow(Display *display, Window root, Window activeWindow);
};

#endif //WINDOW_MANAGER_EWMH_HPP