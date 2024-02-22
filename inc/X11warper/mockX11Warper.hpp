#include "gmock/gmock.h"
#include "X11warper/baseX11Warper.hpp"

class mockX11Warper : public BaseX11Warper {
public:
	mockX11Warper() = default;
	~mockX11Warper() = default;
	MOCK_METHOD(Display *, openDisplay, (), (override));
	MOCK_METHOD(Display *, openDisplay, (const char *), (override));
	MOCK_METHOD(void, closeDisplay, (Display *), (override));
	MOCK_METHOD(int, defaultScreen, (Display *), (override));
	MOCK_METHOD(Window, rootWindow, (Display *, int), (override));
	MOCK_METHOD(Atom, internAtom, (Display *, const char *, bool), (override));
	MOCK_METHOD(int, displayWidth, (Display *, int), (override));
	MOCK_METHOD(int, displayHeight, (Display *, int), (override));
	MOCK_METHOD(int, grabServer, (Display *), (override));
	MOCK_METHOD(int, ungrabServer, (Display *), (override));
	MOCK_METHOD(int, flush, (Display *), (override));
	MOCK_METHOD(XErrorHandler, setErrorHandler, (XErrorHandler), (override));
	MOCK_METHOD(int, selectInput, (Display *, Window, long), (override));
	MOCK_METHOD(int, sync, (Display *, bool), (override));
	MOCK_METHOD(int, queryTree, (Display *, Window, Window *, Window *, Window **, unsigned int *), (override));
	MOCK_METHOD(int, freeX, (void *), (override));
	MOCK_METHOD(int, nextEvent, (Display *, XEvent *), (override));
	MOCK_METHOD(int, sendEvent, (Display *, Window, bool, long, XEvent *), (override));
	MOCK_METHOD(int, changeProperty, (Display *, Window, Atom, Atom, int, int, const unsigned char *, int), (override));
	MOCK_METHOD(int, getProperty, (Display *, Window, Atom, long, long, bool, Atom, Atom *, int *, unsigned long *, unsigned long *, unsigned char **), (override));
	MOCK_METHOD(int, setInputFocus, (Display *, Window, int, Time), (override));
	MOCK_METHOD(int, getErrorText, (Display *, int, char *, int), (override));
	MOCK_METHOD(int, mapWindow, (Display *, Window), (override));
	MOCK_METHOD(int, unmapWindow, (Display *, Window), (override));
	MOCK_METHOD(int, configureWindow, (Display *, Window, unsigned, XWindowChanges *), (override));
	MOCK_METHOD(int, setInputFocus, (Display *, Window, int), (override));
	MOCK_METHOD(int, raiseWindow, (Display *, Window), (override));
	MOCK_METHOD(int, lowerWindow, (Display *, Window), (override));
	MOCK_METHOD(int, setWindowBorder, (Display *, Window, unsigned long), (override));
	MOCK_METHOD(int, getWindowProperty, (Display *, Window, Atom, long, long, bool, Atom, Atom *, int *, unsigned long *, unsigned long *, unsigned char **), (override));
	MOCK_METHOD(int, destroyWindow, (Display *, Window), (override));
	MOCK_METHOD(int, getWindowAttributes, (Display *, Window, XWindowAttributes *), (override));
	MOCK_METHOD(int, addToSaveSet, (Display *, Window), (override));
	MOCK_METHOD(int, removeFromSaveSet, (Display *, Window), (override));
	MOCK_METHOD(int, reparentWindow, (Display *, Window, Window, int, int), (override));
	MOCK_METHOD(int, grabButton, (Display *, unsigned int, unsigned int, Window, bool, unsigned int, int, int, Window, Cursor), (override));
	MOCK_METHOD(int, ungrabButton, (Display *, unsigned int, unsigned int, Window), (override));
	MOCK_METHOD(int, grabKey, (Display *, int, unsigned int, Window, bool, int, int), (override));
	MOCK_METHOD(int, ungrabKey, (Display *, int, unsigned int, Window), (override));
	MOCK_METHOD(int, moveWindow, (Display *, Window, int, int), (override));
	MOCK_METHOD(int, resizeWindow, (Display *, Window, unsigned int, unsigned int), (override));
	MOCK_METHOD(int, keysymToKeycode, (Display *, int), (override));
	MOCK_METHOD(KeySym, stringToKeysym, (const char *), (override));
	MOCK_METHOD(Window, createWindow, (Display *, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual *, unsigned long, XSetWindowAttributes *), (override));
	MOCK_METHOD(int, clearWindow, (Display *, Window), (override));
	MOCK_METHOD(int, drawString, (Display *, Window, GC, int, int, const char *, int), (override));
	MOCK_METHOD(int, createSimpleWindow, (Display *, Window, int, int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long), (override));
};