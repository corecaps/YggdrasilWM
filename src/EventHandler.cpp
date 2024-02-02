/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
 * @file EventHandler.cpp
 * @brief EventHandler class implementation.
 * @date 2021-06-23
 *
 */
#include "EventHandler.hpp"

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
/**
 * @brief Construct a new Event Handler:: Event Handler object
 *
 * @param wm Constant reference to the WindowManager object.
 * @param logger Constant reference to the Logger object.
 * @return EventHandler object.
 *
 */

EventHandler::EventHandler(WindowManager &wm, const Logger &logger)
	:	wm_(wm),
		logger_(logger),
		drag_start_pos_(0, 0),
		drag_start_frame_pos_(0, 0),
		drag_start_frame_size_(0, 0)
		{
	for (auto & i : eventHandlerArray) {
		i = &EventHandler::handleUnknown;
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
EventHandler::~EventHandler() = default;
/**
 * @brief Dispatches the event to the appropriate handler function.
 * the eventHandlerArray is an array of pointers to member functions of the EventHandler class.
 * The array is initialized with the handleUnknown function.
 * The array is then populated with the appropriate handler functions.
 *
 * @param event The XEvent to be dispatched.
 *
 */
void EventHandler::dispatchEvent(const XEvent &event) {
	std::string name = GetEventTypeName(event.xany.type);
	if (event.type > 0 && event.type < LASTEvent && eventHandlerArray[event.type] != nullptr)
		(this->*eventHandlerArray[event.type])(event);
	else
		logger_.Log("Unknown event type: ["  + std::to_string(event.type) + "]\t" + name, L_WARNING);
}

/**
 * @brief Handles the MapNotify event.
 * The MapNotify event is sent to a client when it is mapped.
 * The event handler retrieves the client from the WindowManager object and sets its mapped attribute to true.
 * The event handler then logs the event.
 *
 * @param event The MapNotify event to be handled.
 *
 */
void EventHandler::handleMapNotify(const XEvent &event) {
	auto e = event.xmap;
	Client * client = wm_.getClient(e.window);
	if (client == nullptr) {
		logger_.Log("Ignoring map for unknown window: " + std::to_string(e.window), L_INFO);
		return;
	}
	else {
		logger_.Log("Window Mapped: " + client->getTitle() , L_INFO);
		if (client->getFrame() != e.window) {
			client->restack();
			client->setMapped(true);
		}
	}
}
/**
 * @brief Handles the UnmapNotify event.
 * The UnmapNotify event is sent to a client when it is unmapped.
 * The event handler retrieves the client from the WindowManager object and sets its mapped attribute to false.
 * The event handler then logs the event.
 *
 * @param event The UnmapNotify event to be handled.
 *
 */
void EventHandler::handleUnmapNotify(const XEvent &event) {
	auto e = event.xunmap;
	if (e.event == wm_.getRoot()) {
		logger_.Log("Ignoring unmap for root window", L_INFO);
		return;
	}
	try {
		Client &client = wm_.getClientRef(e.window);
		logger_.Log("Unmapping window: " + client.getTitle(), L_INFO);
		wm_.getLayoutManager()->removeClient(&client);
		client.unframe();
		wm_.getClients().erase(e.window);
		delete &client;
	}
	catch (std::out_of_range &err) {
		logger_.Log("Unmapping unknown window: " + std::to_string(e.window), L_WARNING);
	}
}

void EventHandler::handleConfigureRequest(const XEvent &event) {
	auto e = event.xconfigurerequest;
	XWindowChanges changes;
	changes.x = e.x;
	changes.y = e.y;
	changes.width = e.width;
	changes.height = e.height;
	changes.border_width = e.border_width;
	changes.sibling = e.above;
	changes.stack_mode = e.detail;
	XConfigureWindow(wm_.getDisplay(),e.window,e.value_mask,&changes);
	std::stringstream msg;
	msg << "x:" << changes.x << " y:" << changes.y
		<< "\twidth:" << changes.width << " height:" << changes.height
		<< "\t sibling" << changes.sibling << " stack mode :" << changes.stack_mode
		<< "\t value mask" << e.value_mask << std::endl;
	logger_.Log("Configure request for : "+ std::to_string(e.window), L_INFO);
	logger_.Log(msg.str(),L_INFO);

}
void EventHandler::handleConfigureNotify(const XEvent &event) {
}
/**
 * @brief Handles the ButtonPress event.
 * The ButtonPress event is sent to a client when a mouse button is pressed.
 *
 * @param event The ButtonPress event to be handled.
 *
 */
void EventHandler::handleButtonPress(const XEvent &event) {
	auto e = event.xbutton;
	const Window frame = wm_.getClient(event.xbutton.window)->getFrame();
	// give focus to the window
	XSetInputFocus(wm_.getDisplay(), frame, RevertToParent, CurrentTime);
	// 1. Save initial cursor position.
//	drag_start_pos_ = Position<int>(e.x_root, e.y_root);
//	// 2. Save initial window info.
//	Window returned_root;
//	int x, y;
//	unsigned width, height, border_width, depth;
//	XGetGeometry(
//			wm_.getDisplay(),
//			frame,
//			&returned_root,
//			&x, &y,
//			&width, &height,
//			&border_width,
//			&depth);
//	drag_start_frame_pos_ = Position<int>(x, y);
//	drag_start_frame_size_ = Size<int>(width, height);

	// 3. Raise clicked window to top.
	XRaiseWindow(wm_.getDisplay(),frame);
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
	auto e = event.xexpose;
	auto bar = wm_.getBar();
	if (e.window == bar) {
		auto display_ = wm_.getDisplay();
		auto screen = DefaultScreen(display_);
		std::stringstream message;
		message << PROGRAM_NAME << " " << PROGRAM_VERSION << " " << wm_.getClientCount() << " clients" ;
		XClearWindow(display_, bar);
		XDrawString(wm_.getDisplay(), wm_.getBar(), DefaultGC(display_, screen), 300, 15, message.str().c_str(), message.str().size());
		XFlush(display_);
	}
}
void EventHandler::handleFocusIn(const XEvent &event) {
	auto e = event.xfocus;
	if (e.window == wm_.getRoot()) {
		return;
	}
	Client * client = wm_.getClient(e.window);
	if (client == nullptr) {
		return;
	}
	else {
		logger_.Log("Window focused: " + client->getTitle() , L_INFO);
		client->setFocused(true);
		XSetWindowBorder(wm_.getDisplay(), client->getFrame(), 0x00FF00);
		XFlush(wm_.getDisplay());
	}
}
void EventHandler::handleFocusOut(const XEvent &event) {
	auto e = event.xfocus;
	if (e.window == wm_.getRoot()) {
		return;
	}
	Client * client = wm_.getClient(e.window);
	if (client == nullptr) {
		return;
	}
	else {
		logger_.Log("Window unfocused: " + client->getTitle() , L_INFO);
		client->setFocused(false);
		XSetWindowBorder(wm_.getDisplay(), client->getFrame(), 0x0000FF);
		XFlush(wm_.getDisplay());
	}
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
/**
 * @brief Handles the MapRequest event.
 * The MapRequest event is sent to a client when it is mapped.
 * The event handler retrieves the client from the WindowManager object and sets its mapped attribute to true.
 * The event handler then logs the event.
 *
 * @param event The MapRequest event to be handled.
 *
 */
void EventHandler::handleMapRequest(const XEvent &event) {
	XMapRequestEvent e = event.xmaprequest;
	if (e.parent != wm_.getRoot()) {
		logger_.Log("Ignoring map request for window: " + std::to_string(e.window), L_INFO);
		return;
	}
	try {
		if (wm_.getClientRef(e.window).isMapped()) {
			logger_.Log("Window already mapped: " + std::to_string(e.window), L_INFO);
			return;
		}
		Client &client = wm_.getClientRef(e.window);
		if (client.isFramed()) {
			logger_.Log("Window already framed: " + std::to_string(e.window), L_INFO);
			return;
		}
		else {
			logger_.Log("Framing window: " + client.getTitle(), L_INFO);
			if (client.frame() != YGG_CLI_NO_ERROR)
				logger_.Log("Failed to frame window: " + std::to_string(e.window), L_ERROR);
			wm_.setFocus(&client);
		}
		logger_.Log("Window already mapped: " + std::to_string(e.window), L_INFO);
		return;
	}
	catch (std::out_of_range &err) {
		logger_.Log("Creating new client for window: " + std::to_string(e.window), L_INFO);
		wm_.insertClient(e.window);
		if (wm_.getClientRef(e.window).frame() != YGG_CLI_NO_ERROR)
			logger_.Log("Failed to frame window: " + std::to_string(e.window), L_ERROR);
	}
	XMapWindow(wm_.getDisplay(), e.window);
}

void EventHandler::handleMotionNotify(const XEvent &event) {
	auto e = event.xmotion;
	const Window frame = wm_.getClientRef(event.xmotion.window).getFrame();
//	const Position<int> drag_pos(e.x_root, e.y_root);
//	const Vector2D<int> delta = drag_pos - drag_start_pos_;
//
//	if (e.state & Button1Mask) {
//		// alt + left button: Move window.
//		const Position<int> dest_frame_pos = drag_start_frame_pos_ + delta;
//		XMoveWindow(
//				wm_.getDisplay(),
//				frame,
//				dest_frame_pos.x, dest_frame_pos.y);
//	} else if (e.state & Button3Mask) {
//		// alt + right button: Resize window.
//		// Window dimensions cannot be negative.
//		const Vector2D<int> size_delta(
//				std::max(delta.x, -drag_start_frame_size_.width),
//				std::max(delta.y, -drag_start_frame_size_.height));
//		const Size<int> dest_frame_size = drag_start_frame_size_ + size_delta;
//		// 1. Resize frame.
//		XResizeWindow(
//				wm_.getDisplay(),
//				frame,
//				dest_frame_size.width, dest_frame_size.height);
//		// 2. Resize client window.
//		XResizeWindow(
//				wm_.getDisplay(),
//				e.window,
//				dest_frame_size.width, dest_frame_size.height);
//	}
}
void EventHandler::handleCreateNotify(const XEvent &event) {
	// TODO IMPLEMENT
}

void EventHandler::handleUnknown(const XEvent &event) {
	logger_.Log("Unknown event type: " + std::to_string(event.type), L_WARNING);
}

