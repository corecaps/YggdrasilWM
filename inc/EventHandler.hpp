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
 * this class dispatches the XEvents to the appropriate handler function.
 * it is also responsible for handling the bars drawing.
  * The event handlers functions are private and are called by the dispatchEvent function.
 * @date 2024-02-12
 * @see WindowManager
 * @see Client
 * @see Logger
 */
#ifndef YGGDRASILWM_EVENTHANDLER_HPP
#define YGGDRASILWM_EVENTHANDLER_HPP
extern "C" {
#include <X11/Xutil.h>
}
#include "WindowManager.hpp"
/**
 * @class EventHandler
 * @brief EventHandler class.
 * This class is responsible for handling the XEvents.
 * It is a part of the WindowManager class
 * The event handlers functions are private and are called by the dispatchEvent function.
 * @todo see the pertinence of implementing observer pattern here
 * @see WindowManager
 * @see Client
 * @see Logger
 */
class EventHandler {
public:
	EventHandler(const EventHandler&) = delete;
	EventHandler& operator=(const EventHandler&) = delete;
/**
 * @fn static void EventHandler::create()
 * @brief create the Event handler:: Event handler object
 */
	static void create();
/**
 * @fn EventHandler * EventHandler::getInstance()
 * @brief Get the Event handler:: Event handler object
 */
	static EventHandler * getInstance();
/**
 * @fn ~EventHandler()
 * @brief Destroy the Event handler:: Event handler object
 */
	~EventHandler();
/**
 * @fn void EventHandler::dispatchEvent(const XEvent& event)
 * @brief Dispatches the event to the appropriate handler function.
 * the eventHandlerArray is an array of pointers to member functions of the EventHandler class.
 * The array is initialized with the handleUnknown function.
 * The array is then populated with the appropriate handler functions.
 * @param event The XEvent to be dispatched.
 */
	void dispatchEvent(const XEvent& event);
private:
	using handler = void (EventHandler::*)(const XEvent&);
	handler eventHandlerArray[LASTEvent]{};
	static EventHandler *					instance_;
/**
 * @fn EventHandler(WindowManager &wm, const Logger &logger)
 * @brief Construct a new Event handler:: Event handler object
 * @param wm Constant reference to the WindowManager object.
 * @param logger Constant reference to the Logger object.
 * @return EventHandler object.
 */
	EventHandler();
/**
 * @fn void EventHandler::handleMapNotify(const XEvent& event)
 * @brief Handles the MapNotify event.
 * The MapNotify event is sent to a client when it is mapped.
 * The event handler retrieves the client from the WindowManager object and sets its mapped attribute to true.
 * The event handler then logs the event.
 * the client is restacked
 * @param event The MapNotify event to be handled.
 */
	void handleMapNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleUnmapNotify(const XEvent& event)
 * @brief Handles the UnmapNotify event.
 * The UnmapNotify event is sent to a client when it is unmapped.
 * The event handler retrieves the client from the WindowManager object and sets its mapped attribute to false.
 * The event handler then logs the event.
 * @param event The UnmapNotify event to be handled.
 */
	void handleUnmapNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleConfigureRequest(const XEvent& event)
 * @brief Handles the ConfigureRequest event.
 * The ConfigureRequest event is sent to a client when it requests a change in its size, position, or stacking order.
 * @param event
 */
	void handleConfigureRequest(const XEvent& event);
/**
 * @fn void EventHandler::handleConfigureNotify(const XEvent& event)
 * @brief Handles the ConfigureNotify event.
 * @param event
 */
	void handleConfigureNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleButtonPress(const XEvent& event)
 * @brief Handles the ButtonPress event.
 * The ButtonPress event is sent to a client when a mouse button is pressed.
 * @param event The ButtonPress event to be handled.
 *
 */
	void handleButtonPress(const XEvent& event);
/**
 * @fn void EventHandler::handleButtonRelease(const XEvent& event)
 * @brief Handles the ButtonRelease event.
 * @param event
 */
	void handleButtonRelease(const XEvent& event);
/**
 * @fn void EventHandler::handleKeyPress(const XEvent& event)
 * @brief Handles the KeyPress event.
 * the handling of KeyPress event is delegated to the ConfigDataBindings object
 * to avoid dependencies to the Binding class and the Command class.
 * @param event
 */
	void handleKeyPress(const XEvent& event);
/**
 * @fn void EventHandler::handleKeyRelease(const XEvent& event)
 * @brief Handles the KeyRelease event.
 * @param event
 */
	void handleKeyRelease(const XEvent& event);
/**
 * @fn void EventHandler::handleEnterNotify(const XEvent& event)
 * @brief Handles the EnterNotify event.
 * @param event
 */
	void handleEnterNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleLeaveNotify(const XEvent& event)
 * @brief Handles the LeaveNotify event.
 * @param event
 */
	void handleLeaveNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleExpose(const XEvent& event)
 * @brief This where the bar is drawn, other windows are ignored.
 */
	void handleExpose(const XEvent& event);
/**
 * @fn void EventHandler::handleFocusIn(const XEvent& event)
 * @brief Handles the FocusIn event. Changes the border color of the client.
 * @param event
 */
	void handleFocusIn(const XEvent& event);
/**
 * @fn void EventHandler::handleFocusOut(const XEvent& event)
 * @brief Handles the FocusOut event. Changes the border color of the client.
 * @param event
 */
	void handleFocusOut(const XEvent& event);
/**
 * @fn void EventHandler::handlePropertyNotify(const XEvent& event)
 * @brief Handles the PropertyNotify event.
 * @param event
 */
	void handlePropertyNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleClientMessage(const XEvent& event)
 * @brief Handles the ClientMessage event.
 * @param event
 */
	void handleClientMessage(const XEvent& event);
/**
 * @fn void EventHandler::handleDestroyNotify(const XEvent& event)
 * @brief Handles the DestroyNotify event.
 * @param event
 */
	void handleDestroyNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleReparentNotify(const XEvent& event)
 * @brief Handles the ReparentNotify event.
 * @param event
 */
	void handleReparentNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleMapRequest(const XEvent& event)
 * @brief Handles the MapRequest event.
 * The MapRequest event is sent to a client when it is mapped.
 * The event handler retrieves the client from the WindowManager object and sets its mapped attribute to true.
 * The event handler then logs the event.
 * @param event The MapRequest event to be handled.
 */
	void handleMapRequest(const XEvent& event);
/**
 * @fn void EventHandler::handleMotionNotify(const XEvent& event)
 * @brief Handles the MotionNotify event. this was previously used for dragging windows.r
 * @todo : rework this function to handle dragging windows.
 * @param event
 */
	void handleMotionNotify(const XEvent& event);
/**
 * @fn void EventHandler::handleUnknown(const XEvent& event)
 * @brief Handles the Unknown event.
 * @param event
 */
	void handleUnknown(const XEvent& event);
/**
 * @fn void EventHandler::handleCreateNotify(const XEvent& event)
 * @brief Handles the CreateNotify event.
 * @param event
 */
	void handleCreateNotify(const XEvent &event);
};
#endif //YGGDRASILWM_EVENTHANDLER_HPP
