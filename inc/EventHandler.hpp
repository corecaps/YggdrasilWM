//
// Created by corecaps on 11/22/23.
//

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
