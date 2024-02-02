/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
 * @file Client.cpp
 * @brief Client class implementation.
 * @date 2021-06-23
 *
 */

#include <X11/keysym.h>
#include <iostream>
#include <cstring>
#include "Client.hpp"

/**
 * @brief Client constructor.get window class and title
 * @param display
 * @param root
 * @param window
 */
#include "TreeLayoutManager.hpp"
Client::Client(Display *display, Window root, Window window, TreeLayoutManager *layout_manager)
		: display_(display),
		  layout_manager_(layout_manager),
		  root_(root),
		  window_(window),
		  frame_(0),
		  border_width(0),
		  border_color(0),
		  focused(false),
		  framed(false),
		  mapped(false)
{
	Atom wmClassAtom = XInternAtom(display, "WM_CLASS", False);
	if (wmClassAtom == None) {
		std::cerr << "Failed to intern WM_CLASS atom." << std::endl;
	}
	Atom actualType;
	int actualFormat;
	unsigned long nItems, bytesAfter;
	unsigned char* propData;
	if (XGetWindowProperty(display, window, wmClassAtom, 0, 1024, False, AnyPropertyType,
						   &actualType, &actualFormat, &nItems, &bytesAfter, &propData) != Success) {
		std::cerr << "Failed to get WM_CLASS property." << std::endl;
	}
	Atom XA_STRING = XInternAtom(display, "STRING", False);
	if (actualType == XA_STRING && actualFormat == 8 && nItems > 1) {
		// The WM_CLASS property contains two null-terminated strings.
		char* instanceName = reinterpret_cast<char*>(propData);
		char* className = instanceName + strlen(instanceName) + 1;
		this->class_ = className;
		this->title_ = instanceName;
	} else {
		std::cerr << "Unexpected format or size of WM_CLASS property." << std::endl;
	}
	XFree(propData);
}
Client::~Client() {
	if (this->framed) {
		XDestroyWindow(display_, frame_);
	}
	std::cerr << "Client destroyed :" << title_ << std::endl;
}

/**
 * @brief Client::frame create a frame around the client window, Map the frame,
 * Add the window to the save set Reparent it, grab the buttons
 */

Client_Err Client::frame() {
	const unsigned int BORDER_WIDTH = 3;
	const unsigned long BORDER_COLOR = 0xff0000;
	const unsigned long BG_COLOR = 0x0000ff;

	if (this->framed)
		return(YGG_CLI_LOG_ALREADY_FRAMED);
	XWindowAttributes x_window_attrs;
	if (XGetWindowAttributes(display_, window_, &x_window_attrs) == 0)
		return (YGG_CLI_ERR_RETRIEVE_ATTR);
	if (x_window_attrs.override_redirect)
		return(YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT);
	this->frame_ = XCreateSimpleWindow(
			display_,
			root_,
			x_window_attrs.x,
			x_window_attrs.y,
			x_window_attrs.width,
			x_window_attrs.height,
			BORDER_WIDTH,
			BORDER_COLOR,
			BG_COLOR
			);
		XSelectInput(
			display_,
			this->frame_,
			SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask);
	XAddToSaveSet(display_,window_);
	XReparentWindow(
			display_,
			window_,
			frame_,
			0,0
			);
	XMapWindow(display_,frame_);
	// 9. Grab universal window management actions on client window.
	//   a. Move windows with alt + left button.
	XGrabButton(
			display_,
			Button1,
			Mod1Mask,
			window_,
			false,
			ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
			GrabModeAsync,
			GrabModeAsync,
			None,
			None);
	//   b. Resize windows with alt + right button.
	XGrabButton(
			display_,
			Button3,
			Mod1Mask,
			window_,
			false,
			ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
			GrabModeAsync,
			GrabModeAsync,
			None,
			None);
	//   c. Kill windows with alt + f4.
	XGrabKey(
			display_,
			XKeysymToKeycode(display_, XK_F4),
			Mod1Mask,
			window_,
			false,
			GrabModeAsync,
			GrabModeAsync);
	//   d. Switch windows with alt + tab.
	XGrabKey(
			display_,
			XKeysymToKeycode(display_, XK_Tab),
			Mod1Mask,
			window_,
			false,
			GrabModeAsync,
			GrabModeAsync);
	this->framed = true;
	this->layout_manager_->addClient(this);
	return YGG_CLI_NO_ERROR;
}

void Client::restack() {
	if (this->framed) {
		XRaiseWindow(display_, frame_);
	}
	XRaiseWindow(display_, window_);
}

/**
 * @brief Client::unframe unframe the client window by removing the frame and reparenting the window to the root window
 */

Client_Err Client::unframe() {
	if (!this->framed)
		return(YGG_CLI_LOG_IGNORE_NOT_FRAMED);
//	XUnmapWindow(display_,frame_);
//	XReparentWindow(
//			display_,
//			window_,
//			root_,
//			0,0);
//	XRemoveFromSaveSet(display_,window_);
	XDestroyWindow(display_,frame_);
	this->framed = false;
	this->frame_ = 0;
	return YGG_CLI_NO_ERROR;
}
/**
 * @brief Client::getWindow return the window attribute of the client
 * @return Window
 */
Window Client::getWindow() const {
	return this->window_;
}
/**
 * @brief Client::getError Return a string from an error of Client_Err enum
 * @param error
 * @return String of error
 */
std::string Client::getError(Client_Err error) {
	switch (error) {
		case YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT:
			return "Client::Frame ignoring window with override redirect attribute.";
		case YGG_CLI_LOG_ALREADY_FRAMED :
			return "Client::Frame Client has already been framed";
		case YGG_CLI_ERR_RETRIEVE_ATTR:
			return "Client could not retrieve Window Attributes";
		case YGG_CLI_LOG_IGNORE_NOT_FRAMED:
			return"Client::unFrame Client has no frame";
		default:
			return "Unknown error from Client Class";
	}
}

/**
 * @brief Client::getFrame() return the frame Window of the client
 * @return Window (frame)
 */
Window Client::getFrame() const {
	return frame_;
}
bool Client::isFramed() const {
	return framed;
}
bool Client::isFocused() const {
	return focused;
}

void Client::setFocused(bool f) {
	Client::focused = f;
}

bool Client::isMapped() const {
	return mapped;
}
void Client::setMapped(bool m) {
	Client::mapped = m;
}

const std::string &Client::getTitle() const {
	return title_;
}

const std::string &Client::getClass() const {
	return class_;
}

void Client::move(int x, int y) {
	if (this->framed) {
		XMoveWindow(display_, frame_, x, y);
	}
	else {
		XMoveWindow(display_, window_, x, y);
	}
}

void Client::resize(int width, int height) {
	if (this->framed) {
		XResizeWindow(display_, frame_, width, height);
	}
	XResizeWindow(display_, window_, width , height );
}
