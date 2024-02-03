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
 * @file EventHandler.hpp
 * @brief EventHandler class header.
 * @date 2021-06-23
 * This class is responsible for handling the XEvents.
 * It is a part of the WindowManager class
 * The event handlers functions are private and are called by the dispatchEvent function.
 * @see WindowManager
 * @see Client
 * @see Logger
 */
#ifndef YGGDRASILWM_EVENTHANDLER_HPP
#define YGGDRASILWM_EVENTHANDLER_HPP
extern "C" {
#include <X11/Xutil.h>
}
#include "window_manager.hpp"
#include "Logger.hpp"

class EventHandler {
public:
	EventHandler(WindowManager &wm, const Logger &logger);
	~EventHandler();
	void dispatchEvent(const XEvent& event);
private:
	Position<int> drag_start_pos_;
	Position<int> drag_start_frame_pos_;
	Size<int> drag_start_frame_size_;
	WindowManager wm_;
	const Logger &logger_;
	using Handler = void (EventHandler::*)(const XEvent&);
	Handler eventHandlerArray[LASTEvent]{};
	void handleMapNotify(const XEvent& event);
	void handleUnmapNotify(const XEvent& event);
	void handleConfigureRequest(const XEvent& event);
	void handleConfigureNotify(const XEvent& event);
	void handleButtonPress(const XEvent& event);
	void handleButtonRelease(const XEvent& event);
	void handleKeyPress(const XEvent& event);
	void handleKeyRelease(const XEvent& event);
	void handleEnterNotify(const XEvent& event);
	void handleLeaveNotify(const XEvent& event);
	void handleExpose(const XEvent& event);
	void handleFocusIn(const XEvent& event);
	void handleFocusOut(const XEvent& event);
	void handlePropertyNotify(const XEvent& event);
	void handleClientMessage(const XEvent& event);
	void handleDestroyNotify(const XEvent& event);
	void handleReparentNotify(const XEvent& event);
	void handleMapRequest(const XEvent& event);
	void handleMotionNotify(const XEvent& event);
	void handleUnknown(const XEvent& event);
	void handleCreateNotify(const XEvent &event);

};

#endif //YGGDRASILWM_EVENTHANDLER_HPP
