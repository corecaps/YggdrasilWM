/**
 * @file window_manager.cpp
 * @brief WindowManager class implementation.
 * @date 2021-06-23
 *
 */

#include <iostream>
#include "window_manager.hpp"
#include "EventHandler.hpp"
#include <algorithm>
#include <csignal>
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
	debug_stream << "Opened X display " << XDisplayName(display_c_str);
	logger.Log(debug_stream.str(), L_INFO);
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
		  root_(DefaultRootWindow(display)),
		  WM_PROTOCOLS(XInternAtom(display_, "WM_PROTOCOLS", false)),
		  WM_DELETE_WINDOW(XInternAtom(display_, "WM_DELETE_WINDOW", false)),
		  running(true),
		  layout_manager_(nullptr){
	logger_.Log("Window Manager Created !\n", L_INFO);
}

/**
 * @brief Destroy the Window Manager:: Window Manager object
 * Close the display.
 */

WindowManager::~WindowManager() {
	logger_.Log("Window Manager Destroyed !\n", L_INFO);
	XCloseDisplay(display_);
}

void WindowManager::Init() {
	std::stringstream debug_stream;
	selectEventOnRoot();
	XGrabServer(display_);
	getTopLevelWindows(debug_stream);
	XUngrabServer(display_);
	XFlush(display_);
	this->layout_manager_ = new TreeLayoutManager(this->display_, this->root_);
}

void WindowManager::getTopLevelWindows(std::stringstream &debug_stream) {
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
	debug_stream << "Found " << num_top_level_windows << " top level windows." << "root:" << root_;
	logger_.Log(debug_stream.str(), L_INFO);
	for (unsigned int i = 0; i < num_top_level_windows; ++i) {
		Client *newClient = new Client(display_, root_, top_level_windows[i],layout_manager_);
		Client_Err err = newClient->frame();
		XMapWindow(display_, top_level_windows[i]);
		LogLevel debug_level;
		switch (err) {
			case YGG_CLI_NO_ERROR:
				debug_stream << "Client framed: " << newClient->getTitle() << "\t[" << top_level_windows[i] << "]";
				debug_level = L_INFO;
				break;
			case YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT:
				debug_stream << Client::getError(err) << "\t[" << top_level_windows[i] << "]";
				debug_level = L_WARNING;
				break;
			case YGG_CLI_LOG_ALREADY_FRAMED:
				debug_stream << Client::getError(err) << "\t[" << top_level_windows[i] << "]";
				debug_level = L_WARNING;
				break;
			case YGG_CLI_ERR_RETRIEVE_ATTR:
				debug_stream << Client::getError(err) << "\t[" << top_level_windows[i] << "]";
				debug_level = L_ERROR;
				break;
			case YGG_CLI_LOG_IGNORE_NOT_FRAMED:
				debug_stream << Client::getError(err) << "\t[" << top_level_windows[i] << "]";
				debug_level = L_INFO;
				break;
		}
		logger_.Log(debug_stream.str(), debug_level);
		clients_[newClient->getWindow()] = newClient;
	}
	XFree(top_level_windows);
}

void WindowManager::selectEventOnRoot() const {
	XSetErrorHandler(&WindowManager::OnWMDetected);
	XSelectInput(
			display_,
			root_,
			SubstructureRedirectMask | SubstructureNotifyMask);
	XSync(display_, false);
	XSetErrorHandler(&WindowManager::OnXError);
}

/**
 * @brief Run the window manager
 * This method is the main loop of the window manager.
 * It will handle the events and call the appropriate methods.
 *
 */
void WindowManager::Run() {
	EventHandler eventHandler(*this, logger_);
	while (running) {
		XSync(display_, false);
		XEvent e;
		XNextEvent(display_, &e);
		eventHandler.dispatchEvent(e);
	}
}

int WindowManager::OnXError(Display *display, XErrorEvent *e) {
	const int MAX_ERROR_TEXT_LENGTH = 1024;
	char error_text[MAX_ERROR_TEXT_LENGTH];
	XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
	std::cerr << "Received X error:\n"
			  << "    Request: " << int(e->request_code)
			  << " - " << XRequestCodeToString(e->request_code) << "\n"
			  << "    Error code: " << int(e->error_code)
			  << " - " << error_text << "\n"
			  << "    Resource ID: " << e->resourceid
			  << std::endl;
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

const Logger &WindowManager::getLogger() const {
	return logger_;
}

Display *WindowManager::getDisplay() const {
	return display_;
}

std::unordered_map<Window, Client *> &WindowManager::getClients() {
	return clients_;
}

Client &WindowManager::getClient(Window window) {
	return *clients_.at(window);
}

void WindowManager::insertClient(Window window) {
	std::stringstream debug_stream;
	Client *client = new Client(display_, root_, window, layout_manager_);
	debug_stream << "Inserting client in map: " << client->getTitle() << "\t[" << window << "]";
	logger_.Log(debug_stream.str(), L_INFO);
	clients_.insert({window, client});
}

const Window WindowManager::getRoot() const {
	return root_;
}

bool WindowManager::isFrame(Window window) {
	for (auto &client: clients_) {
		if (client.second->getFrame() == window)
			return true;
	}
	return false;
}
