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
 * @file X11Wrapper.cpp
 * @brief wrap X11 functions.
 * @date 2024-02-11
 */

#include "X11wrapper/X11Wrapper.hpp"

Display *X11Wrapper::openDisplay() {
	return XOpenDisplay(nullptr);
}

Display *X11Wrapper::openDisplay(const char *displayName) {
	return XOpenDisplay(displayName);
}

void X11Wrapper::closeDisplay(Display *display) {
	XCloseDisplay(display);
}

int X11Wrapper::defaultScreen(Display *display) {
	return DefaultScreen(display);
}

Window X11Wrapper::rootWindow(Display *display, int screen) {
	return RootWindow(display, screen);
}

Atom X11Wrapper::internAtom(Display *display, const char *atomName, bool onlyIfExists) {
	return XInternAtom(display, atomName, onlyIfExists);
}

int X11Wrapper::displayWidth(Display *display, int screen) {
	return DisplayWidth(display, screen);
}

int X11Wrapper::displayHeight(Display *display, int screen) {
	return DisplayHeight(display, screen);
}

int X11Wrapper::grabServer(Display *display) {
	return XGrabServer(display);
}

int X11Wrapper::ungrabServer(Display *display) {
	return XUngrabServer(display);
}

int X11Wrapper::flush(Display *display) {
	return XFlush(display);
}

XErrorHandler X11Wrapper::setErrorHandler(XErrorHandler handler) {
	return XSetErrorHandler(handler);
}

int X11Wrapper::selectInput(Display *display, Window window, long eventMask) {
	return XSelectInput(display, window, eventMask);
}

int X11Wrapper::sync(Display *display, bool discard) {
	return XSync(display, discard);
}

int
X11Wrapper::queryTree(Display *display, Window window, Window *rootReturn, Window *parentReturn, Window **childrenReturn,
					  unsigned int *nChildrenReturn) {
	return XQueryTree(display, window, rootReturn, parentReturn, childrenReturn, nChildrenReturn);
}

int X11Wrapper::freeX(void *data) {
	return XFree(data);
}

int X11Wrapper::nextEvent(Display *display, XEvent *eventReturn) {
	return XNextEvent(display, eventReturn);
}

int X11Wrapper::sendEvent(Display *display, Window window, bool propagate, long eventMask, XEvent *event_send) {
	return XSendEvent(display, window, propagate, eventMask, event_send);
}

int X11Wrapper::changeProperty(Display *display, Window window, Atom property, Atom type, int format, int mode,
							   const unsigned char *data, int nelements) {
	return XChangeProperty(display, window, property, type, format, mode, data, nelements);
}

int
X11Wrapper::getProperty(Display *display, Window window, Atom property, long longOffset, long longLength, bool delete_,
						Atom reqType, Atom *actualTypeReturn, int *actualFormatReturn, unsigned long *nitemsReturn,
						unsigned long *bytesAfterReturn, unsigned char **propReturn) {
	return XGetWindowProperty(display, window, property, longOffset, longLength, delete_, reqType, actualTypeReturn,
							  actualFormatReturn, nitemsReturn, bytesAfterReturn, propReturn);
}

int X11Wrapper::setInputFocus(Display *display, Window focus, int revertTo, Time time) {
	return XSetInputFocus(display, focus, revertTo, time);
}

int X11Wrapper::getErrorText(Display *display, int code, char *buffer_return, int length) {
	return XGetErrorText(display, code, buffer_return, length);
}

int X11Wrapper::mapWindow(Display *display, Window window) {
	return XMapWindow(display, window);
}

int X11Wrapper::configureWindow(Display *display, Window window, unsigned int valueMask, XWindowChanges *changes) {
	return XConfigureWindow(display, window, valueMask, changes);
}

int X11Wrapper::unmapWindow(Display *display, Window window) {
	return XUnmapWindow(display, window);
}

int X11Wrapper::setInputFocus(Display *display, Window focus, int revertTo) {
	return XSetInputFocus(display, focus, revertTo, CurrentTime);
}

int X11Wrapper::raiseWindow(Display *display, Window window) {
	return XRaiseWindow(display, window);
}

int X11Wrapper::lowerWindow(Display *display, Window window) {
	return XLowerWindow(display, window);
}

int X11Wrapper::setWindowBorder(Display *display, Window window, unsigned long border) {
	return XSetWindowBorder(display, window, border);
}

int X11Wrapper::getWindowProperty(Display *display, Window window, Atom property, long long_offset, long long_length,
								  bool delete_, Atom req_type, Atom *actual_type_return, int *actual_format_return,
								  unsigned long *nitems_return, unsigned long *bytes_after_return,
								  unsigned char **prop_return) {
	return XGetWindowProperty(display, window, property, long_offset, long_length, delete_, req_type, actual_type_return,
							  actual_format_return, nitems_return, bytes_after_return, prop_return);
}

int X11Wrapper::destroyWindow(Display *display, Window window) {
	return XDestroyWindow(display, window);
}

int X11Wrapper::getWindowAttributes(Display *display, Window window, XWindowAttributes *window_attributes_return) {
	return XGetWindowAttributes(display, window, window_attributes_return);
}

int X11Wrapper::addToSaveSet(Display *display, Window window) {
	return XAddToSaveSet(display, window);
}

int X11Wrapper::removeFromSaveSet(Display *display, Window window) {
	return XRemoveFromSaveSet(display, window);
}

int X11Wrapper::reparentWindow(Display *display, Window window, Window parent, int x, int y) {
	return XReparentWindow(display, window, parent, x, y);
}

int X11Wrapper::grabButton(Display *display, unsigned int button, unsigned int modifiers, Window grabWindow,
						   bool ownerEvents, unsigned int eventMask, int pointerMode, int keyboardMode,
						   Window confineTo, Cursor cursor) {
	return XGrabButton(display, button, modifiers, grabWindow, ownerEvents, eventMask, pointerMode, keyboardMode,
					   confineTo, cursor);
}

int X11Wrapper::ungrabButton(Display *display, unsigned int button, unsigned int modifiers, Window grab_window) {
	return XUngrabButton(display, button, modifiers, grab_window);
}

int X11Wrapper::grabKey(Display *display, int keycode, unsigned int modifiers, Window grabWindow, bool ownerEvents,
						int pointerMode, int keyboardMode) {
	return XGrabKey(display, keycode, modifiers, grabWindow, ownerEvents, pointerMode, keyboardMode);
}

int X11Wrapper::ungrabKey(Display *display, int keycode, unsigned int modifiers, Window grabWindow) {
	return XUngrabKey(display, keycode, modifiers, grabWindow);
}

int X11Wrapper::moveWindow(Display *display, Window window, int x, int y) {
	return XMoveWindow(display, window, x, y);
}

int X11Wrapper::resizeWindow(Display *display, Window window, unsigned int width, unsigned int height) {
	return XResizeWindow(display, window, width, height);
}

int X11Wrapper::keysymToKeycode(Display *display, int keysym) {
	return XKeysymToKeycode(display, keysym);
}

KeySym X11Wrapper::stringToKeysym(const char *string) {
	return XStringToKeysym(string);
}

Window X11Wrapper::createWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height,
								unsigned int border_width, int depth, unsigned int _class, Visual *visual,
								unsigned long valuemask, XSetWindowAttributes *attributes) {
	return XCreateWindow(display, parent, x, y, width, height, border_width, depth, _class, visual, valuemask,
						 attributes);
}

int X11Wrapper::clearWindow(Display *display, Window window) {
	return XClearWindow(display, window);
}

int X11Wrapper::drawString(Display *display, Window window, GC gc, int x, int y, const char *string, int length) {
	return XDrawString(display, window, gc, x, y, string, length);
}

Window
X11Wrapper::createSimpleWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height,
							   unsigned int border_width, unsigned long border, unsigned long background) {
	return XCreateSimpleWindow(display, parent, x, y, width, height, border_width, border, background);
}


