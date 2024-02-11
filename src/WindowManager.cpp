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
 * @date 2024-02-11
 *
 */
#include "WindowManager.hpp"
#include "EventHandler.hpp"
#include "Config/ConfigDataBars.hpp"
#include "Config/ConfigHandler.hpp"
#include "Config/ConfigDataGroups.hpp"

bool WindowManager::wm_detected_;
WindowManager * WindowManager::instance_ = nullptr;
void WindowManager::Create( const std::string &display_str) {
	Logger::GetInstance()->Log("================ Yggdrasil Initialisation ================\n\n", L_INFO);
	std::stringstream debug_stream;
	if (WindowManager::instance_ != nullptr) {
		throw std::runtime_error("WindowManager instance already created");
	}
	const char *display_c_str =
			display_str.empty() ? nullptr : display_str.c_str();
	Display *display = XOpenDisplay(display_c_str);
	if (display == nullptr) {
		Logger::GetInstance()->Log("Failed to open X display " + std::string(XDisplayName(display_c_str)), L_ERROR);
		throw std::runtime_error("Failed to open X display");
	}
	Logger::GetInstance()->Log("Opened X Display:\t" + std::string(XDisplayName(display_c_str)), L_INFO);
	WindowManager::instance_ = new WindowManager(display);
}
WindowManager::WindowManager(Display *display)
		: display_(display),
		  root_(DefaultRootWindow(display)),
		  WM_PROTOCOLS(XInternAtom(display_, "WM_PROTOCOLS", false)),
		  WM_DELETE_WINDOW(XInternAtom(display_, "WM_DELETE_WINDOW", false)),
		  active_group_(nullptr),
		  running(true),
		  bar_(0){}
WindowManager::~WindowManager() {
	for (auto &client: clients_) {
		delete client.second;
	}
	clients_.clear();
	for (auto &group: groups_) {
		delete group;
	}
	groups_.clear();
	XCloseDisplay(display_);
}
void WindowManager::Init() {
	selectEventOnRoot();
	if (wm_detected_) {
		throw std::runtime_error("Another window manager is already running.");
	}
	XGrabServer(display_);
	getTopLevelWindows();
	XUngrabServer(display_);
	XFlush(display_);
	Bar();
	signal(SIGINT, handleSIGHUP);
}
void WindowManager::selectEventOnRoot() const {
	XSetErrorHandler(&WindowManager::OnWMDetected);
	XSelectInput(
			display_,
			root_,
			SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask);
	XSetErrorHandler(&WindowManager::OnXError);
	XGrabKey(display_,
			 XKeysymToKeycode(display_, XK_1),
			 Mod1Mask ,
			 root_,
			 false,
			 GrabModeAsync,
			 GrabModeAsync);
	XGrabKey(display_,
			 XKeysymToKeycode(display_, XK_2),
			 Mod1Mask ,
			 root_,
			 false,
			 GrabModeAsync,
			 GrabModeAsync);
	XSync(display_, false);
}
void WindowManager::getTopLevelWindows() {
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
		throw std::runtime_error("Root window is not the same as the one returned by XQueryTree");
	}
	addGroupsFromConfig();
	Logger::GetInstance()->Log("Found " + std::to_string(num_top_level_windows)+ " top level windows.\troot:" + std::to_string(root_), L_INFO);
	for (unsigned int i = 0; i < num_top_level_windows; ++i) {
		auto *newClient = new Client(display_, root_, top_level_windows[i], active_group_, active_group_->getInactiveColor(), active_group_->getBorderSize());
		Client_Err err = newClient->frame();
		setFocus(newClient);
		if (err == YGG_CLI_ERR_RETRIEVE_ATTR) {
			delete newClient;
			Logger::GetInstance()->Log("Failed to frame client: " + std::to_string(top_level_windows[i]), L_ERROR);
			continue;
		} else if (err == YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT)
		{
			delete newClient;
			Logger::GetInstance()->Log("Ignoring override redirect window: " + std::to_string(top_level_windows[i]), L_WARNING);
			continue;
		} else if (err == YGG_CLI_LOG_ALREADY_FRAMED) {
			delete newClient;
			Logger::GetInstance()->Log("Client already framed: " + std::to_string(top_level_windows[i]), L_WARNING);
			continue;
		}
		clients_[newClient->getWindow()] = newClient;
	}
	XFree(top_level_windows);
}
void WindowManager::addGroupsFromConfig() {
	auto configGroups = ConfigHandler::GetInstance().getConfigData<ConfigDataGroups>()->getGroups();
	for (auto group: configGroups) {
		Group *g = new Group(group);
		groups_.push_back(g);
	}
	groups_[0]->SetActive(true);
	active_group_ = groups_[0];
	Logger::GetInstance()->Log("Active Group is [" + active_group_->GetName() + "]", L_INFO);
}
void WindowManager::Run() {
	Logger::GetInstance()->Log("================ Yggdrasil WM Running ================\n\n", L_INFO);
	EventHandler::create();
	XEvent e;
	while (running && !XNextEvent(display_, &e)) {
		EventHandler::getInstance()->dispatchEvent(e);
		XSync(display_, false);
	}
}
void WindowManager::testRun() {
	try {
		EventHandler::getInstance();
	} catch (const std::exception &e) {
		EventHandler::create();
	}
	XSync(display_, false);
	XEvent ev;
	EventHandler::getInstance()->dispatchEvent(ev);
	XSync(display_, false);
}
void WindowManager::insertClient(Window window) {
	std::stringstream debug_stream;
	int BorderSize = active_group_->getBorderSize();
	unsigned long inactiveColor = active_group_->getInactiveColor();
	auto *client = new Client(display_, root_, window, active_group_, inactiveColor, BorderSize);
	debug_stream << "Inserting client in map: " << client->getTitle() << "\t[" << window << "]";
	Logger::GetInstance()->Log(debug_stream.str(), L_INFO);
	clients_.insert({window, client});
}
void WindowManager::Bar() {
	int screen = DefaultScreen(display_);
//	int barHeight = ConfigHandler::GetInstance().getConfigData<ConfigDataBars>()->getBar(0)->getBarHeight();
	int barHeight = 30;
	bar_ = XCreateSimpleWindow(
			display_,
			root_,
			0,
			0,
			DisplayWidth(display_, screen),
			barHeight,
			0,
			BlackPixel(display_, screen),
			WhitePixel(display_, screen));
	Logger::GetInstance()->Log("Bar created", L_INFO);
	XSelectInput(display_, bar_, ExposureMask | KeyPressMask);
	XMapWindow(display_, bar_);
	XFlush(display_);
}
void WindowManager::setFocus(Client *client) {
	if (client != nullptr) {
		XSetInputFocus(display_, client->getWindow(), RevertToParent, CurrentTime);
	}
}
void WindowManager::Destroy() {
	if (WindowManager::instance_ != nullptr) {
		delete WindowManager::instance_;
		WindowManager::instance_ = nullptr;
	}
}
void handleSIGHUP(int signal) {
	std::cout << "Caught signal " << signal << std::endl;
	if (WindowManager::getInstance() != nullptr) {
		WindowManager::getInstance()->Stop();
	}
}
void WindowManager::Stop() {
	this->running = false;
	std::cout << "Stopping WindowManager" << std::endl;
}
WindowManager * WindowManager::getInstance() {
	if (WindowManager::instance_ != nullptr) {
		return WindowManager::instance_;
	} else {
		throw std::runtime_error("WindowManager instance not created");
	}
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
Display *WindowManager::getDisplay() const { return display_; }
std::unordered_map<Window, Client *> &WindowManager::getClients() { return clients_; }
Client &WindowManager::getClientRef(Window window) { return *clients_.at(window); }
Window WindowManager::getRoot() const { return root_; }
unsigned long WindowManager::getClientCount() { return clients_.size(); }
Window WindowManager::getBar() const { return bar_; }
void WindowManager::setActiveGroup(Group *activeGroup) { active_group_ = activeGroup; }
Group *WindowManager::getActiveGroup() const { return active_group_; }
const std::vector<Group *> &WindowManager::getGroups() const { return groups_; }
bool WindowManager::getRunning() const { return running; }
int WindowManager::OnXError(Display *display, XErrorEvent *e) {
	const int MAX_ERROR_TEXT_LENGTH = 1024;
	char error_text[MAX_ERROR_TEXT_LENGTH];
	XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
	std::stringstream error_stream;
	error_stream << "Received X error:\n"
				 << "    Request: " << int(e->request_code)
				 << "    Error code: " << int(e->error_code)
				 << " - " << error_text << "\n"
				 << "    Resource ID: " << e->resourceid;
	Logger::GetInstance()->Log(error_stream.str(), L_ERROR);
	// The return value is ignored.
	return 0;
}
int WindowManager::OnWMDetected([[maybe_unused]] Display *display, XErrorEvent *e) {
	if (static_cast<int>(e->error_code) == BadAccess)
		wm_detected_ = true;
	return 0;
}
