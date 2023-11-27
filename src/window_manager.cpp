/**
 * @file window_manager.cpp
 * @brief WindowManager class implementation.
 * @date 2021-06-23
 *
 */

#include <iostream>
#include "window_manager.hpp"
#include "EventHandler.hpp"
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include "util.hpp"
extern "C" {
#include <X11/Xutil.h>
}

using ::std::max;
using ::std::mutex;
using ::std::string;
using ::std::unique_ptr;

bool WindowManager::wm_detected_;
mutex WindowManager::wm_detected_mutex_;
/**
 * @brief Create a WindowManager object
 * This function is the only way to instantiate a WindowManager object.
 * It returns a unique_ptr to the object.
 * If the display_str is empty, it will use the DISPLAY environment variable.
 * If the display_str is not empty, it will use the display_str.
 * If the display_str is invalid, it will return nullptr.
 *
 * @param logger
 * @param display_str
 * @return unique_ptr<WindowManager>
 */

unique_ptr<WindowManager> WindowManager::Create(const Logger &logger, const string &display_str) {
	std::stringstream debug_stream;
	const char *display_c_str =
			display_str.empty() ? nullptr : display_str.c_str();
	Display *display = XOpenDisplay(display_c_str);
	if (display == nullptr) {
		debug_stream << "Failed to open X display " << XDisplayName(display_c_str);
		logger.Log(debug_stream.str(), L_ERROR);
		return nullptr;
	}
	return unique_ptr<WindowManager>(new WindowManager(display, logger));
}

/**
 * @brief Construct a new Window Manager:: Window Manager object
 * @param display
 * @param logger
 */

WindowManager::WindowManager(Display *display, const Logger &logger)
		: display_(display),
		  logger_(logger),
		  root_(DefaultRootWindow(display_)),
		  WM_PROTOCOLS(XInternAtom(display_, "WM_PROTOCOLS", false)),
		  WM_DELETE_WINDOW(XInternAtom(display_, "WM_DELETE_WINDOW", false)) {
	logger_.Log("Window Manager Created !\n", L_INFO);
}

/**
 * @brief Destroy the Window Manager:: Window Manager object
 * Close the display.
 */

WindowManager::~WindowManager() {
	XCloseDisplay(display_);
}
/**
 * @brief Run the window manager
 * This method is the main loop of the window manager.
 * It will handle the events and call the appropriate methods.
 *
 */
void WindowManager::Run() {
	std::stringstream debug_stream;
	wm_detected_ = false;
	XSetErrorHandler(&WindowManager::OnWMDetected);
	int ret_code = XSelectInput(
			display_,
			root_,
			SubstructureRedirectMask | SubstructureNotifyMask);
	debug_stream << "XSelectInput returned " << ret_code << std::endl;
	logger_.Log(debug_stream.str(), L_INFO);
	debug_stream.str("");
	debug_stream.clear();
	XSync(display_, false);
	if (wm_detected_) {
		debug_stream << "Detected another window manager on display "
					 << XDisplayString(display_);
		logger_.Log(debug_stream.str(), L_ERROR);
		debug_stream.str("");
		debug_stream.clear();
		return;
	}
	debug_stream << "Starting window manager on display " << XDisplayString(display_) << wm_detected_ << std::endl;
	logger_.Log(debug_stream.str(), L_INFO);
	debug_stream.str("");
	debug_stream.clear();
	XSync(display_, false);
	XSetErrorHandler(&WindowManager::OnXError);
	XGrabServer(display_);
	Window returned_root, returned_parent;
	Window *top_level_windows;
	unsigned int num_top_level_windows;
	XQueryTree(
			display_,
			root_,
			&returned_root,
			&returned_parent,
			&top_level_windows,
			&num_top_level_windows);
	if (returned_root != root_) {
		debug_stream << "Root window is not the same as the one returned by XQueryTree" << std::endl;
		logger_.Log(debug_stream.str(), L_ERROR);
		debug_stream.str("");
		debug_stream.clear();
	}
	for (unsigned int i = 0; i < num_top_level_windows; ++i) {
		Client* newClient = new Client(display_,root_,top_level_windows[i]);
		newClient->frame();
		clients_[newClient->getWindow()] = newClient;
	}
	XFree(top_level_windows);
	XUngrabServer(display_);
	EventHandler eventHandler(*this,logger_);
	while(1) {
		XSync(display_, false);
		XEvent e;
		XNextEvent(display_, &e);
		eventHandler.dispatchEvent(e);
	}
}
//
//void WindowManager::OnCreateNotify(const XCreateWindowEvent &e) {}
//
//void WindowManager::OnDestroyNotify(const XDestroyWindowEvent &e) {}
//
//void WindowManager::OnReparentNotify(const XReparentEvent &e) {}
//
//void WindowManager::OnMapNotify(const XMapEvent &e) {}
//
//void WindowManager::OnUnmapNotify(const XUnmapEvent &e) {
//	// If the window is a client window we manage, unframe it upon UnmapNotify. We
//	// need the check because we will receive an UnmapNotify event for a frame
//	// window we just destroyed ourselves.
//	if (!clients_.count(e.window)) {
//		std::cout << "Ignore UnmapNotify for non-client window " << e.window;
//		return;
//	}
//
//	// Ignore event if it is triggered by reparenting a window that was mapped
//	// before the window manager started.
//	//
//	// Since we receive UnmapNotify events from the SubstructureNotify mask, the
//	// event attribute specifies the parent window of the window that was
//	// unmapped. This means that an UnmapNotify event from a normal client window
//	// should have this attribute set to a frame window we maintain. Only an
//	// UnmapNotify event triggered by reparenting a pre-existing window will have
//	// this attribute set to the root window.
//	if (e.event == root_) {
//		std::cout << "Ignore UnmapNotify for reparented pre-existing window "
//				  << e.window;
//		return;
//	}
//
//	Unframe(e.window);
//}
//
//void WindowManager::OnConfigureNotify(const XConfigureEvent &e) {}
//
//void WindowManager::OnMapRequest(const XMapRequestEvent &e) {
//	// 1. Frame or re-frame window.
//	Frame(e.window, false);
//	// 2. Actually map window.
//	XMapWindow(display_, e.window);
//}
//
//void WindowManager::OnConfigureRequest(const XConfigureRequestEvent &e) {
//	std::stringstream debug_stream;
//	XWindowChanges changes;
//	changes.x = e.x;
//	changes.y = e.y;
//	changes.width = e.width;
//	changes.height = e.height;
//	changes.border_width = e.border_width;
//	changes.sibling = e.above;
//	changes.stack_mode = e.detail;
//	if (clients_.count(e.window)) {
//		const Window frame = clients_[e.window];
//		XConfigureWindow(display_, frame, e.value_mask, &changes);
//		debug_stream << "Resize [" << frame << "] to " << Size<int>(e.width, e.height);
//	}
//	XConfigureWindow(display_, e.window, e.value_mask, &changes);
//	debug_stream << "Resize " << e.window << " to " << Size<int>(e.width, e.height);
//	logger_.Log(debug_stream.str(), L_INFO);
//}
//
//void WindowManager::OnButtonPress(const XButtonEvent &e) {
//	//CHECK(clients_.count(e.window));
//	const Window frame = clients_[e.window];
//
//	// 1. Save initial cursor position.
//	drag_start_pos_ = Position<int>(e.x_root, e.y_root);
//
//	// 2. Save initial window info.
//	Window returned_root;
//	int x, y;
//	unsigned width, height, border_width, depth;
//	XGetGeometry(
//			display_,
//			frame,
//			&returned_root,
//			&x, &y,
//			&width, &height,
//			&border_width,
//			&depth);
//	drag_start_frame_pos_ = Position<int>(x, y);
//	drag_start_frame_size_ = Size<int>(width, height);
//
//	// 3. Raise clicked window to top.
//	XRaiseWindow(display_, frame);
//}
//
//void WindowManager::OnButtonRelease(const XButtonEvent &e) {}
//
//void WindowManager::OnMotionNotify(const XMotionEvent &e) {
//	//CHECK(clients_.count(e.window));
//	const Window frame = clients_[e.window];
//	const Position<int> drag_pos(e.x_root, e.y_root);
//	const Vector2D<int> delta = drag_pos - drag_start_pos_;
//
//	if (e.state & Button1Mask) {
//		// alt + left button: Move window.
//		const Position<int> dest_frame_pos = drag_start_frame_pos_ + delta;
//		XMoveWindow(
//				display_,
//				frame,
//				dest_frame_pos.x, dest_frame_pos.y);
//	} else if (e.state & Button3Mask) {
//		// alt + right button: Resize window.
//		// Window dimensions cannot be negative.
//		const Vector2D<int> size_delta(
//				max(delta.x, -drag_start_frame_size_.width),
//				max(delta.y, -drag_start_frame_size_.height));
//		const Size<int> dest_frame_size = drag_start_frame_size_ + size_delta;
//		// 1. Resize frame.
//		XResizeWindow(
//				display_,
//				frame,
//				dest_frame_size.width, dest_frame_size.height);
//		// 2. Resize client window.
//		XResizeWindow(
//				display_,
//				e.window,
//				dest_frame_size.width, dest_frame_size.height);
//	}
//}
//
//void WindowManager::OnKeyPress(const XKeyEvent &e) {
//	if ((e.state & Mod1Mask) &&
//		(e.keycode == XKeysymToKeycode(display_, XK_F4))) {
//		// alt + f4: Close window.
//		//
//		// There are two ways to tell an X window to close. The first is to send it
//		// a message of type WM_PROTOCOLS and value WM_DELETE_WINDOW. If the client
//		// has not explicitly marked itself as supporting this more civilized
//		// behavior (using XSetWMProtocols()), we kill it with XKillClient().
//		Atom *supported_protocols;
//		int num_supported_protocols;
//		if (XGetWMProtocols(display_,
//							e.window,
//							&supported_protocols,
//							&num_supported_protocols) &&
//			(::std::find(supported_protocols,
//						 supported_protocols + num_supported_protocols,
//						 WM_DELETE_WINDOW) !=
//			 supported_protocols + num_supported_protocols)) {
//			std::cout << "Gracefully deleting window " << e.window;
//			// 1. Construct message.
//			XEvent msg;
//			memset(&msg, 0, sizeof(msg));
//			msg.xclient.type = ClientMessage;
//			msg.xclient.message_type = WM_PROTOCOLS;
//			msg.xclient.window = e.window;
//			msg.xclient.format = 32;
//			msg.xclient.data.l[0] = WM_DELETE_WINDOW;
//			// 2. Send message to window to be closed.
//			//CHECK(XSendEvent(display_, e.window, false, 0, &msg));
//		} else {
//			std::cout << "Killing window " << e.window;
//			XKillClient(display_, e.window);
//		}
//	} else if ((e.state & Mod1Mask) &&
//			   (e.keycode == XKeysymToKeycode(display_, XK_Tab))) {
//		// alt + tab: Switch window.
//		// 1. Find next window.
//		auto i = clients_.find(e.window);
//		//CHECK(i != clients_.end());
//		++i;
//		if (i == clients_.end()) {
//			i = clients_.begin();
//		}
//		// 2. Raise and set focus.
//		XRaiseWindow(display_, i->second);
//		XSetInputFocus(display_, i->first, RevertToPointerRoot, CurrentTime);
//	}
//}
//
//void WindowManager::OnKeyRelease(const XKeyEvent &e) {}

int WindowManager::OnXError(Display *display, XErrorEvent *e) {
	const int MAX_ERROR_TEXT_LENGTH = 1024;
	char error_text[MAX_ERROR_TEXT_LENGTH];
	XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
	std::cerr << "Received X error:\n"
			  << "    Request: " << int(e->request_code)
			  << " - " << XRequestCodeToString(e->request_code) << "\n"
			  << "    Error code: " << int(e->error_code)
			  << " - " << error_text << "\n"
			  << "    Resource ID: " << e->resourceid;
	// The return value is ignored.
	return 0;
}

int WindowManager::OnWMDetected(Display *display, XErrorEvent *e) {
	// In the case of an already running window manager, the error code from
	// XSelectInput is BadAccess. We don't expect this handler to receive any
	// other errors.
	if (static_cast<int>(e->error_code) == BadAccess)
		wm_detected_ = true;
	// The return value is ignored.
	return 0;
}
