/**
 * @file EventHandler.cpp
 * @brief EventHandler class implementation.
 * @date 2021-06-23
 *
 */
#include "EventHandler.hpp"

/**
 * @brief Construct a new Event Handler:: Event Handler object
 *
 * @param wm Constant reference to the WindowManager object.
 * @param logger Constant reference to the Logger object.
 * @return EventHandler object.
 *
 */

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
	eventHandlerArray[CreateNotify] = &EventHandler::handleCreateNotify;
}
EventHandler::~EventHandler() {}
/**
 * @brief Dispatches the event to the appropriate handler function.
 * the eventHandlerArray is an array of pointers to member functions of the EventHandler class.
 * The array is initialized with the handleUnknown function.
 * The array is then populated with the appropriate handler functions.
 *
 * @param event The XEvent to be dispatched.
 *
 */
std::string GetEventTypeName(int eventType) {
    std::string name;
    switch (eventType) {
        case CreateNotify:        name = "CreateNotify";         break;
        case DestroyNotify:       name = "DestroyNotify";        break;
        case UnmapNotify:         name = "UnmapNotify";          break;
        case MapNotify:           name = "MapNotify";            break;
        case MapRequest:          name = "MapRequest";           break;
        case ReparentNotify:      name = "ReparentNotify";       break;
        case ConfigureNotify:     name = "ConfigureNotify";      break;
        case ConfigureRequest:    name = "ConfigureRequest";     break;
        case GravityNotify:       name = "GravityNotify";        break;
        case ResizeRequest:       name = "ResizeRequest";        break;
        case CirculateNotify:     name = "CirculateNotify";      break;
        case CirculateRequest:    name = "CirculateRequest";     break;
        case PropertyNotify:      name = "PropertyNotify";       break;
        case SelectionClear:      name = "SelectionClear";       break;
        case SelectionRequest:    name = "SelectionRequest";     break;
        case SelectionNotify:     name = "SelectionNotify";      break;
        case ColormapNotify:      name = "ColormapNotify";       break;
        case ClientMessage:       name = "ClientMessage";        break;
        case MappingNotify:       name = "MappingNotify";        break;
        case KeyPress:            name = "KeyPress";             break;
        case KeyRelease:          name = "KeyRelease";           break;
        case ButtonPress:         name = "ButtonPress";          break;
        case ButtonRelease:       name = "ButtonRelease";        break;
        case MotionNotify:        name = "MotionNotify";         break;
        case EnterNotify:         name = "EnterNotify";          break;
        case LeaveNotify:         name = "LeaveNotify";          break;
        case FocusIn:             name = "FocusIn";              break;
        case FocusOut:            name = "FocusOut";             break;
        case Expose:              name = "Expose";               break;
        case GraphicsExpose:      name = "GraphicsExpose";       break;
        case NoExpose:            name = "NoExpose";             break;
        case VisibilityNotify:    name = "VisibilityNotify";     break;
        case GenericEvent:        name = "GenericEvent";         break;
        case LASTEvent:           name = "LASTEvent";            break;
        default:                  name = "Unknown";              break;
    }
    return name;
}

void EventHandler::dispatchEvent(const XEvent &event) {

	std::string name = GetEventTypeName(event.xany.type);
	logger_.Log("Dispatching event: [" + std::to_string(event.type) + "]\t"+ name , L_INFO);
	if (event.type > 0 && event.type < LASTEvent && eventHandlerArray[event.type] != nullptr)
		(this->*eventHandlerArray[event.type])(event);
	else
		logger_.Log("Unknown event type: ["  + std::to_string(event.type) + "]\t" + name, L_WARNING);
}
void EventHandler::handleMapNotify(const XEvent &event) {
	// TODO IMPLEMENT
	}
void EventHandler::handleUnmapNotify(const XEvent &event) {
	auto e = event.xunmap;
	if (e.event != wm_.getRoot()) {
		logger_.Log("Ignoring unmap for root window", L_INFO);
		return;
	}	auto &clients = wm_.getClients();
	if (clients.find(e.window) != clients.end()) {
		logger_.Log("Unmapping window: " + std::to_string(e.window), L_INFO);
		clients.at(e.window)->unframe();
	}
	else
		logger_.Log("Unmapping unknown window: " + std::to_string(e.window), L_WARNING);
}
void EventHandler::handleConfigureRequest(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleConfigureNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleButtonPress(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleButtonRelease(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleKeyPress(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleKeyRelease(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleEnterNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleLeaveNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleExpose(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleFocusIn(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleFocusOut(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handlePropertyNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleClientMessage(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleDestroyNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleReparentNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleMapRequest(const XEvent &event) {
	XMapRequestEvent e = event.xmaprequest;
	if (e.parent != wm_.getRoot()) {
		logger_.Log("Ignoring map request for window: " + std::to_string(e.window), L_INFO);
		return;
	}
	auto &clients = wm_.getClients();
	if (clients.find(e.window) != clients.end()) {
		logger_.Log("Mapping window: " + std::to_string(e.window), L_INFO);
		clients.at(e.window)->frame();
	}
	else
		logger_.Log("Mapping unknown window: " + std::to_string(e.window), L_WARNING);
	XMapWindow(wm_.getDisplay(), e.window);
}

void EventHandler::handleMotionNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleCreateNotify(const XEvent &event) {
	// TODO IMPLEMENT
}
void EventHandler::handleUnknown(const XEvent &event) {
	logger_.Log("Unknown event type: " + std::to_string(event.type), L_WARNING);
}

