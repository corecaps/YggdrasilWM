/**
 * @file Client.cpp
 * @brief Client class implementation.
 * @date 2021-06-23
 *
 */

#include <X11/keysym.h>
#include "Client.hpp"

Client::Client(Display *display, Window root, Window window)
		: display_(display),
		  root_(root),
		  window_(window),
		  frame_(0),
		  border_width(0),
		  border_color(0),
		  focused(false),
		  framed(false)
{}
Client::~Client() = default;

/**
 *
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
	frame_ = XCreateSimpleWindow(
			display_,
			root_,
			x_window_attrs.x,
			x_window_attrs.y,
			x_window_attrs.width +4,
			x_window_attrs.height+4,
			BORDER_WIDTH,
			BORDER_COLOR,
			BG_COLOR
			);
	XSelectInput(
			display_,
			frame_,
			SubstructureRedirectMask | SubstructureNotifyMask);
	XAddToSaveSet(display_,frame_);
	XReparentWindow(
			display_,
			window_,
			frame_,
			2,2
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
}

/**
 *
 */

Client_Err Client::unframe() {
	if (!this->framed)
		return(YGG_CLI_LOG_IGNORE_NOT_FRAMED);
	XUnmapWindow(display_,frame_);
	XReparentWindow(
			display_,
			window_,
			root_,
			0,0);
	XRemoveFromSaveSet(display_,window_);
	XDestroyWindow(display_,frame_);
}

Window Client::getWindow() {
	return this->window_;
}


