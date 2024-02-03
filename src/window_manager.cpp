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

extern "C" {
#include <X11/Xutil.h>
}

using ::std::max;
using ::std::string;
using ::std::unique_ptr;

bool WindowManager::wm_detected_;
static WindowManager *windowManagerInstance = nullptr;

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

unique_ptr<WindowManager> WindowManager::Create(Logger &logger,ConfigHandler& configHandler, const string &display_str) {
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
	return unique_ptr<WindowManager>(new WindowManager(display, logger, configHandler));
}

/**
 * @brief Construct a new Window Manager:: Window Manager object
 * @param display
 * @param logger
 */

WindowManager::WindowManager(Display *display, const Logger &logger, ConfigHandler &configHandler)
		: display_(display),
		  logger_(logger),
		  configHandler_(configHandler),
		  root_(DefaultRootWindow(display)),
		  WM_PROTOCOLS(XInternAtom(display_, "WM_PROTOCOLS", false)),
		  WM_DELETE_WINDOW(XInternAtom(display_, "WM_DELETE_WINDOW", false)),
		  running(true),
		  layout_manager_(nullptr) {
	logger_.Log("Window Manager Created !\n", L_INFO);
}

bool WindowManager::getRunning() const {
	return running;
}

/**
 * @brief Destroy the Window Manager:: Window Manager object
 * Close the display.
 */

WindowManager::~WindowManager() {
	delete layout_manager_;
	XCloseDisplay(display_);
}

void handleSIGHUP(int signal) {
	if (windowManagerInstance != nullptr) {
		windowManagerInstance->Stop();
	}
}

void WindowManager::Stop() {
	running = false;
}

void WindowManager::Init() {
	std::stringstream debug_stream;
	selectEventOnRoot();
	if (wm_detected_) {
		throw std::runtime_error("Another window manager is already running.");
	}
	XGrabServer(display_);
	getTopLevelWindows(debug_stream);
	XUngrabServer(display_);
	XFlush(display_);
	windowManagerInstance = this;
	Bar();
	signal(SIGINT, handleSIGHUP);
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
	debug_stream << "Found " << num_top_level_windows << " top level windows." << "root:" << root_ << std::endl;
	int BorderSize = std::get<int>(configHandler_.getConfig("BorderWidth"));
	int size_x = DisplayWidth(display_, DefaultScreen(display_)) - 2*BorderSize;
	int size_y = DisplayHeight(display_, DefaultScreen(display_)) -30-2*BorderSize;
	int pos_x = 0;
	int pos_y = 30;
	this->layout_manager_ = new TreeLayoutManager(this->display_, this->root_, size_x, size_y, pos_x, pos_y);
	logger_.Log(debug_stream.str(), L_INFO);
	unsigned long ActiveColor = std::get<unsigned long>(configHandler_.getConfig("ActiveColor"));
	for (unsigned int i = 0; i < num_top_level_windows; ++i) {
		Client *newClient = new Client(display_, root_, top_level_windows[i], layout_manager_, ActiveColor, BorderSize);
		Client_Err err = newClient->frame();
		setFocus(newClient);
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
			SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask);
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

Client &WindowManager::getClientRef(Window window) {
	return *clients_.at(window);
}

Client *WindowManager::getClient(Window window) {
	auto clientIter = clients_.find(window);
	if (clientIter != clients_.end()) {
		return clientIter->second;
	}
	for (const auto &client: clients_) {
		if (client.second->getFrame() == window) {
			return client.second;
		}
	}
	return nullptr;
}

void WindowManager::insertClient(Window window) {
	std::stringstream debug_stream;
	unsigned long ActiveColor = std::get<unsigned long>(configHandler_.getConfig("ActiveColor"));
	int BorderSize = std::get<int>(configHandler_.getConfig("BorderWidth"));
	Client *client = new Client(display_, root_, window, layout_manager_, ActiveColor, BorderSize);
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

TreeLayoutManager *WindowManager::getLayoutManager() const {
	return layout_manager_;
}

void WindowManager::setFocus(Client *client) {
	if (client != nullptr) {
		XSetInputFocus(display_, client->getWindow(), RevertToParent, CurrentTime);
	}
}

void WindowManager::Bar() {
	int screen = DefaultScreen(display_);
	bar_ = XCreateSimpleWindow(
			display_,
			root_,
			0,
			0,
			DisplayWidth(display_, screen),
			20,
			0,
			BlackPixel(display_, screen),
			WhitePixel(display_, screen));
	logger_.Log("Bar created", L_INFO);
	XSelectInput(display_, bar_, ExposureMask | KeyPressMask);
	XMapWindow(display_, bar_);
	XFlush(display_);
}

Window WindowManager::getBar() const {
	return bar_;
}

int WindowManager::getClientCount() {

	return clients_.size();
}

ConfigHandler &WindowManager::getConfigHandler() {
	return configHandler_;
}


