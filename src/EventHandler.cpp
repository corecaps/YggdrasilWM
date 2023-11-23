//
// Created by corecaps on 11/22/23.
//
#include "EventHandler.hpp"

EventHandler::EventHandler(const WindowManager &wm, const Logger &logger)
	:	wm_(wm),
		logger_(logger) {
	for (int i = 0; i < LASTEvent; i++) {
		eventHandlerArray[i] = &EventHandler::handleUnknown;
	}
	eventHandlerArray[MapNotify] = &EventHandler::handleMapNotify;
	eventHandlerArray[UnmapNotify] = &EventHandler::handleUnmapNotify;
	eventHandlerArray[ConfigureNotify] = &EventHandler::handleConfigureNotify;
	eventHandlerArray[ConfigureRequest] = &EventHandler::handleConfigureRequest;
	eventHandlerArray[ButtonPress] = &EventHandler::handleButtonPress;
	eventHandlerArray[ButtonRelease] = &EventHandler::handleButtonRelease;
	eventHandlerArray[KeyPress] = &EventHandler::handleKeyPress;
	eventHandlerArray[KeyRelease] = &EventHandler::handleKeyRelease;
	eventHandlerArray[EnterNotify] = &EventHandler::handleEnterNotify;
	eventHandlerArray[LeaveNotify] = &EventHandler::handleLeaveNotify;
	eventHandlerArray[Expose] = &EventHandler::handleExpose;
	eventHandlerArray[FocusIn] = &EventHandler::handleFocusIn;
	eventHandlerArray[FocusOut] = &EventHandler::handleFocusOut;
	eventHandlerArray[PropertyNotify] = &EventHandler::handlePropertyNotify;
	eventHandlerArray[ClientMessage] = &EventHandler::handleClientMessage;
	eventHandlerArray[DestroyNotify] = &EventHandler::handleDestroyNotify;
	eventHandlerArray[ReparentNotify] = &EventHandler::handleReparentNotify;
	eventHandlerArray[MapRequest] = &EventHandler::handleMapRequest;
	eventHandlerArray[MotionNotify] = &EventHandler::handleMotionNotify;
}
EventHandler::~EventHandler() {}
void EventHandler::dispatchEvent(const XEvent &event) {

	if (event.type > 0 && event.type < LASTEvent && eventHandlerArray[event.type] != nullptr)
		(this->*eventHandlerArray[event.type])(event);
	else
		logger_.Log("Unknown event type: " + std::to_string(event.type), L_WARNING);
}
void EventHandler::handleMapNotify(const XEvent &event) {}
void EventHandler::handleUnmapNotify(const XEvent &event) {}
void EventHandler::handleConfigureRequest(const XEvent &event) {}
void EventHandler::handleConfigureNotify(const XEvent &event) {}
void EventHandler::handleButtonPress(const XEvent &event) {}
void EventHandler::handleButtonRelease(const XEvent &event) {}
void EventHandler::handleKeyPress(const XEvent &event) {}
void EventHandler::handleKeyRelease(const XEvent &event) {}
void EventHandler::handleEnterNotify(const XEvent &event) {}
void EventHandler::handleLeaveNotify(const XEvent &event) {}
void EventHandler::handleExpose(const XEvent &event) {}
void EventHandler::handleFocusIn(const XEvent &event) {}
void EventHandler::handleFocusOut(const XEvent &event) {}
void EventHandler::handlePropertyNotify(const XEvent &event) {}
void EventHandler::handleClientMessage(const XEvent &event) {}
void EventHandler::handleDestroyNotify(const XEvent &event) {}
void EventHandler::handleReparentNotify(const XEvent &event) {}
void EventHandler::handleMapRequest(const XEvent &event) {}
void EventHandler::handleMotionNotify(const XEvent &event) {}
void EventHandler::handleUnknown(const XEvent &event) {
	logger_.Log("Unknown event type: " + std::to_string(event.type), L_WARNING);
}