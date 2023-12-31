/**
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
	EventHandler(const WindowManager &wm, const Logger &logger);
	~EventHandler();
	void dispatchEvent(const XEvent& event);
private:
	const WindowManager &wm_;
	const Logger &logger_;
	using Handler = void (EventHandler::*)(const XEvent&);
	Handler eventHandlerArray[LASTEvent];
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
};

#endif //YGGDRASILWM_EVENTHANDLER_HPP
