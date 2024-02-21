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
 * @file EventHandler.cpp
 * @brief EventHandler class implementation.
 * @date 2024-02-12
 */
#include "EventHandler.hpp"
#include "Logger.hpp"
#include "Config/ConfigDataBindings.hpp"
#include "Group.hpp"
#include <string>
extern "C" {
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
};
#include "Ewmh.hpp"

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
EventHandler * EventHandler::instance_ = nullptr;
void EventHandler::create() {
	if (instance_ != nullptr) {
		throw std::runtime_error("EventHandler instance already created");
	}
	instance_ = new EventHandler();
}
EventHandler *EventHandler::getInstance() {
	if (instance_ == nullptr) {
		throw std::runtime_error("EventHandler instance not created");
	}
	return instance_;
}
EventHandler::EventHandler() {
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
void EventHandler::dispatchEvent(const XEvent &event) {
	std::string name = GetEventTypeName(event.xany.type);
	if (event.type > 0 && event.type < LASTEvent && eventHandlerArray[event.type] != nullptr)
		(this->*eventHandlerArray[event.type])(event);
	else
		Logger::GetInstance()->Log("Unknown event type: ["  + std::to_string(event.type) + "]\t" + name, L_WARNING);
}
void EventHandler::handleMapNotify(const XEvent &event) {
	auto e = event.xmap;
	Client * client = WindowManager::getInstance()->getClient(e.window);
	if (client == nullptr) {
		Logger::GetInstance()->Log("Ignoring map for unknown window: " + std::to_string(e.window), L_INFO);
		return;
	}
	else {
		Logger::GetInstance()->Log("Window Mapped: " + client->getTitle() , L_INFO);
		if (client->getFrame() != e.window) {
			client->restack();
			client->setMapped(true);
		}
	}
}
void EventHandler::handleUnmapNotify(const XEvent &event) {
	auto e = event.xunmap;
	if (e.event == WindowManager::getInstance()->getRoot()) {
		ConfigHandler::GetInstance().getConfigData<ConfigDataBindings>()->grabKeys(WindowManager::getInstance()->getDisplay(),WindowManager::getInstance()->getRoot());
		Logger::GetInstance()->Log("Ignoring unmap for root window", L_INFO);
		return;
	}
	try {
		Client &client = WindowManager::getInstance()->getClientRef(e.window);
		Logger::GetInstance()->Log("Unmapping window: " + client.getTitle(), L_INFO);
		if (client.getFrame() != e.window) {
			client.setMapped(false);
		}
		else {
			client.unframe();
			client.getGroup()->removeClient(&client);
			WindowManager::getInstance()->getClients().erase(e.window);
			delete &client;
		}
	}
	catch (std::out_of_range &err) {
		Logger::GetInstance()->Log("Unmapping unknown window: " + std::to_string(e.window), L_WARNING);
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
	XConfigureWindow(WindowManager::getInstance()->getDisplay(),e.window,e.value_mask,&changes);
}
void EventHandler::handleConfigureNotify(const XEvent &event) {
}
void EventHandler::handleButtonPress(const XEvent &event) {
	auto e = event.xbutton;
	const Window frame = WindowManager::getInstance()->getClient(event.xbutton.window)->getFrame();
	// give focus to the window
	XSetInputFocus(WindowManager::getInstance()->getDisplay(), frame, RevertToParent, CurrentTime);
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
	XRaiseWindow(WindowManager::getInstance()->getDisplay(),frame);
}
void EventHandler::handleButtonRelease(const XEvent &event) {}
void EventHandler::handleKeyPress(const XEvent &event) {
	auto e = &event.xkey;
	ConfigHandler::GetInstance().getConfigData<ConfigDataBindings>()->handleKeypressEvent(e);
	XSync(WindowManager::getInstance()->getDisplay(),false);
}
void EventHandler::handleKeyRelease(const XEvent &event) {}
void EventHandler::handleEnterNotify(const XEvent &event) {}
void EventHandler::handleLeaveNotify(const XEvent &event) {}
void EventHandler::handleExpose(const XEvent &event) {
	auto e = event.xexpose;
	const std::vector<Window> &windows = Bars::getInstance().getWindows();
	for (auto window : windows) {
		if (window == e.window) {
			Bars::getInstance().redraw();
			return;
		}
	}
}
void EventHandler::handleFocusIn(const XEvent &event) {
	auto e = event.xfocus;
	WindowManager::getInstance()->setActiveWindow(e.window);
	ewmh::updateActiveWindow(WindowManager::getInstance()->getDisplay(), WindowManager::getInstance()->getRoot(), e.window);
	if (e.window == WindowManager::getInstance()->getRoot()) {
		return;
	}
	Client * client = WindowManager::getInstance()->getClient(e.window);
	if (client == nullptr) {
		return;
	}
	else {
		unsigned long ActiveColor = client->getGroup()->getActiveColor();
		Logger::GetInstance()->Log("Window focused: " + client->getTitle() , L_INFO);
		XSetWindowBorder(WindowManager::getInstance()->getDisplay(), client->getFrame(), ActiveColor);
		XFlush(WindowManager::getInstance()->getDisplay());
	}
}
void EventHandler::handleFocusOut(const XEvent &event) {
	auto e = event.xfocus;
	if (WindowManager::getInstance()->getActiveWindow() == e.window) {
		WindowManager::getInstance()->setActiveWindow(0);
		ewmh::updateActiveWindow(WindowManager::getInstance()->getDisplay(), WindowManager::getInstance()->getRoot(), None);
	}
	if (e.window == WindowManager::getInstance()->getRoot()) {
		return;
	}
	Client * client = WindowManager::getInstance()->getClient(e.window);
	if (client == nullptr) {
		return;
	}
	else {
		unsigned long InActiveColor = client->getGroup()->getInactiveColor();
		Logger::GetInstance()->Log("Window unfocused: " + client->getTitle() , L_INFO);
		XSetWindowBorder(WindowManager::getInstance()->getDisplay(), client->getFrame(), InActiveColor);
		XFlush(WindowManager::getInstance()->getDisplay());
	}
}
void EventHandler::handlePropertyNotify(const XEvent &event) {
	XPropertyEvent e = event.xproperty;
	Logger::GetInstance()->Log("PropertyNotify: " + std::to_string(e.atom), L_INFO);
}
void EventHandler::handleClientMessage(const XEvent &event) {
	XClientMessageEvent e = event.xclient;
	Display * display = WindowManager::getInstance()->getDisplay();
	Window root = WindowManager::getInstance()->getRoot();
	ewmh::handleMessage(&e, display, root);
}
void EventHandler::handleDestroyNotify(const XEvent &event) {
	auto e = event.xdestroywindow;
	if (e.window == WindowManager::getInstance()->getRoot()) {
		Logger::GetInstance()->Log("Ignoring destroy for root window", L_INFO);
		return;
	}
	try {
		Client &client = WindowManager::getInstance()->getClientRef(e.window);
		Logger::GetInstance()->Log("Destroying window: " + client.getTitle(), L_INFO);
		client.unframe();
		WindowManager::getInstance()->getClients().erase(e.window);
		client.getGroup()->removeClient(&client);
		delete &client;
		auto clients = WindowManager::getInstance()->getActiveGroup()->getClients();
		if (!clients.empty()) {
			Client *dclient = clients.begin()->second;
			if (dclient != nullptr) {
				WindowManager::getInstance()->setFocus(dclient);
				return;
			}
		}
		XSetInputFocus(WindowManager::getInstance()->getDisplay(),
					   WindowManager::getInstance()->getRoot(),
					   RevertToParent, CurrentTime);
	} catch (std::out_of_range &err) {
		Logger::GetInstance()->Log("Destroying unknown window: " + std::to_string(e.window), L_WARNING);
	}
}
void EventHandler::handleReparentNotify(const XEvent &event) {}
void EventHandler::handleMapRequest(const XEvent &event) {
	XMapRequestEvent e = event.xmaprequest;
	if (e.parent != WindowManager::getInstance()->getRoot()) {
		Logger::GetInstance()->Log("Ignoring map request for window: " + std::to_string(e.window), L_INFO);
		return;
	}
	try {
		if (WindowManager::getInstance()->getClientRef(e.window).isMapped()) {
			Logger::GetInstance()->Log("Window already mapped: " + std::to_string(e.window), L_INFO);
			return;
		}
		Client &client = WindowManager::getInstance()->getClientRef(e.window);
		if (client.isFramed()) {
			Logger::GetInstance()->Log("Window already framed: " + std::to_string(e.window), L_INFO);
			return;
		}
		else {
			Logger::GetInstance()->Log("Framing window: " + client.getTitle(), L_INFO);
			if (client.frame() != YGG_CLI_NO_ERROR)
				Logger::GetInstance()->Log("Failed to frame window: " + std::to_string(e.window), L_ERROR);
			WindowManager::getInstance()->setFocus(&client);
		}
		Logger::GetInstance()->Log("Window already mapped: " + std::to_string(e.window), L_INFO);
		return;
	}
	catch (std::out_of_range &err) {
		Logger::GetInstance()->Log("Creating new client for window: " + std::to_string(e.window), L_INFO);
		WindowManager::getInstance()->insertClient(e.window);
		if (WindowManager::getInstance()->getClientRef(e.window).frame() != YGG_CLI_NO_ERROR)
			Logger::GetInstance()->Log("Failed to frame window: " + std::to_string(e.window), L_ERROR);
	}
	XMapWindow(WindowManager::getInstance()->getDisplay(), e.window);
}
void EventHandler::handleMotionNotify(const XEvent &event) {
	auto e = event.xmotion;
	const Window frame = WindowManager::getInstance()->getClientRef(event.xmotion.window).getFrame();
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
void EventHandler::handleCreateNotify(const XEvent &event) {}
void EventHandler::handleUnknown(const XEvent &event) {}


