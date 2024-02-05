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
 * @date 2024-02-04
 *
 */


#include "window_manager.hpp"

#include "EventHandler.hpp"
using ::std::max;
using ::std::string;
using ::std::unique_ptr;

bool WindowManager::wm_detected_;
static WindowManager *windowManagerInstance = nullptr;

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
WindowManager::WindowManager(Display *display,
							 const Logger &logger,
							 ConfigHandler &configHandler)
		: display_(display),
		  logger_(logger),
		  configHandler_(configHandler),
		  root_(DefaultRootWindow(display)),
		  WM_PROTOCOLS(XInternAtom(display_, "WM_PROTOCOLS", false)),
		  WM_DELETE_WINDOW(XInternAtom(display_, "WM_DELETE_WINDOW", false)),
		  running(true),
		  bar_(0){
	logger_.Log("Window Manager Created !\n", L_INFO);
}
bool WindowManager::getRunning() const { return running; }
WindowManager::~WindowManager() {
	XCloseDisplay(display_);
}
void handleSIGHUP(int signal) {
	std::cout << "Caught signal " << signal << std::endl;
	if (windowManagerInstance != nullptr) {
		windowManagerInstance->Stop();
	}
}
void WindowManager::Stop() {
	this->running = false;
	std::cout << "Stopping WindowManager" << std::endl;
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
	int BarHeight = std::get<int>(configHandler_.getConfig("BarHeight"));
	groups_.emplace_back("default", BorderSize, 0, BarHeight, *this, LayoutType::TREE);
//	this->layout_manager_ = new TreeLayoutManager(this->display_, this->root_, size_x, size_y, pos_x, pos_y);
	logger_.Log(debug_stream.str(), L_INFO);
	unsigned long InActiveColor = std::get<unsigned long>(configHandler_.getConfig("InActiveColor"));
	for (unsigned int i = 0; i < num_top_level_windows; ++i) {
		auto *newClient = new Client(display_, root_, top_level_windows[i], &groups_[0], InActiveColor, BorderSize);
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
void WindowManager::Run() {
	EventHandler eventHandler(*this, logger_);
	XEvent e;
	while (running && !XNextEvent(display_, &e)) {
		eventHandler.dispatchEvent(e);
		XSync(display_, false);
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
	if (static_cast<int>(e->error_code) == BadAccess)
		wm_detected_ = true;
	return 0;
}
const Logger &WindowManager::getLogger() const { return logger_; }
Display *WindowManager::getDisplay() const { return display_; }
std::unordered_map<Window, Client *> &WindowManager::getClients() { return clients_; }
Client &WindowManager::getClientRef(Window window) { return *clients_.at(window); }
Window WindowManager::getRoot() const { return root_; }
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
	unsigned long InActiveColor = std::get<unsigned long>(configHandler_.getConfig("InActiveColor"));
	int BorderSize = std::get<int>(configHandler_.getConfig("BorderWidth"));
	auto *client = new Client(display_, root_, window, &groups_[0], InActiveColor, BorderSize);
	debug_stream << "Inserting client in map: " << client->getTitle() << "\t[" << window << "]";
	logger_.Log(debug_stream.str(), L_INFO);
	clients_.insert({window, client});
}
bool WindowManager::isFrame(Window window) {
	for (auto &client: clients_) {
		if (client.second->getFrame() == window)
			return true;
	}
	return false;
}
Window WindowManager::getBar() const { return bar_; }
unsigned long WindowManager::getClientCount() { return clients_.size(); }
ConfigHandler &WindowManager::getConfigHandler() { return configHandler_; }
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
