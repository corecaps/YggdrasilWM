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
 * @file Ewmh.cpp
 * @brief ewmh namespace implementation.
 * @date 2024-02-11
 */
#include "Ewmh.hpp"
#include <vector>

namespace ewmh {
	void initEwmh(Display *display, Window root) {
		Atom netSupported = XInternAtom(display, "_NET_SUPPORTED", False);
		std::vector<Atom> supportedAtoms = {
				XInternAtom(display, "_NET_WM_NAME", False),
				XInternAtom(display, "_NET_WM_DESKTOP", False),
				XInternAtom(display, "_NET_ACTIVE_WINDOW",False),
				XInternAtom(display, "_NET_NUMBER_OF_DESKTOPS",False),
				XInternAtom(display, "_NET_WM_STATE",False)
				// Add other supported atoms here
		};

		// Register _NET_SUPPORTED property
		XChangeProperty(
				display,                        // Display
				root,                           // Root window
				netSupported,                    // Property to change
				XA_ATOM,                         // Type of property
				32,                              // Format of the property (32-bit)
				PropModeReplace,                 // Replace existing property
				reinterpret_cast<unsigned char*>(supportedAtoms.data()), // New value
				supportedAtoms.size()            // Number of elements in the new value
		);

		XFlush(display);
	}
}