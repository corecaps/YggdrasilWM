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
 * @file X11Warper.hpp
 * @brief warp X11 functions.
 * @date 2024-02-11
 */

#ifndef X11_WARPER_HPP
#define X11_WARPER_HPP
#include "X11warper/baseX11Warper.hpp"
extern "C" {
#include <X11/Xlib.h>
#include <X11/Xatom.h>
}
#include <memory>
#include <string>

class X11Warper : public BaseX11Warper {
public:
	X11Warper() = default;
	~X11Warper() = default;
	Display * openDisplay() override;
	Display * openDisplay(const char * displayName) override;
	void closeDisplay(Display * display) override;
	int defaultScreen(Display * display) override;
	Window rootWindow(Display * display, int screen) override;
	Atom internAtom(Display * display, const char * atomName, bool onlyIfExists) override;
	int displayWidth(Display * display, int screen) override;
	int displayHeight(Display * display, int screen) override;
	int grabServer(Display * display) override;
	int ungrabServer(Display * display) override;
	int flush(Display * display) override;
	XErrorHandler setErrorHandler(XErrorHandler handler) override;
	int selectInput(Display * display, Window window, long eventMask) override;
	int sync(Display * display, bool discard) override;
	int queryTree(Display * display, Window window, Window * rootReturn, Window * parentReturn, Window ** childrenReturn, unsigned int * nChildrenReturn) override;
	int freeX(void * data) override;
	int nextEvent(Display * display, XEvent * eventReturn) override;
	int sendEvent(Display * display, Window window, bool propagate, long eventMask, XEvent * event_send) override;
	int changeProperty(Display * display, Window window, Atom property, Atom type, int format, int mode, const unsigned char * data, int nelements) override;
	int getProperty(Display * display, Window window, Atom property, long longOffset, long longLength, bool delete_, Atom reqType, Atom * actualTypeReturn, int * actualFormatReturn, unsigned long * nitemsReturn, unsigned long * bytesAfterReturn, unsigned char ** propReturn) override;
	int setInputFocus(Display * display, Window focus, int revertTo, Time time) override;
	int getErrorText(Display * display, int code, char * buffer_return, int length) override;
	int mapWindow(Display * display, Window window) override;
	int unmapWindow(Display * display, Window window) override;
	int configureWindow(Display * display, Window window, unsigned valueMask, XWindowChanges * changes) override;
	int setInputFocus(Display * display, Window focus, int revertTo) override;
	int raiseWindow(Display * display, Window window) override;
	int lowerWindow(Display * display, Window window) override;
	int setWindowBorder(Display * display, Window window, unsigned long border) override;
	int getWindowProperty(Display * display, Window window, Atom property, long long_offset, long long_length, bool delete_, Atom req_type, Atom * actual_type_return, int * actual_format_return, unsigned long * nitems_return, unsigned long * bytes_after_return, unsigned char ** prop_return) override;
	int destroyWindow(Display * display, Window window) override;
	int getWindowAttributes(Display * display, Window window, XWindowAttributes * window_attributes_return) override;
	int addToSaveSet(Display * display, Window window) override;
	int removeFromSaveSet(Display * display, Window window) override;
	int reparentWindow(Display * display, Window window, Window parent, int x, int y) override;
	int grabButton(Display * display, unsigned int button, unsigned int modifiers, Window grabWindow, bool ownerEvents, unsigned int eventMask, int pointerMode, int keyboardMode, Window confineTo, Cursor cursor) override;
	int ungrabButton(Display * display, unsigned int button, unsigned int modifiers, Window grab_window) override;
	int grabKey(Display * display, int keycode, unsigned int modifiers, Window grabWindow, bool ownerEvents, int pointerMode, int keyboardMode) override;
	int ungrabKey(Display * display, int keycode, unsigned int modifiers, Window grabWindow) override;
	int moveWindow(Display * display, Window window, int x, int y) override;
	int resizeWindow(Display * display, Window window, unsigned int width, unsigned int height) override;
	int keysymToKeycode(Display * display, int keysym) override;
	KeySym stringToKeysym(const char * string) override;
	Window createWindow(Display * display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int _class, Visual * visual, unsigned long valuemask, XSetWindowAttributes * attributes) override;
	int clearWindow(Display * display, Window window) override;
	int drawString(Display * display, Window window, GC gc, int x, int y, const char * string, int length) override;
	int createSimpleWindow(Display * display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, unsigned long border, unsigned long background) override;
};

#endif //X11_WARPER_HPP