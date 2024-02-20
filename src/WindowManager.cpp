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
#include "Config/ConfigDataBindings.hpp"
#include "Bars/Bars.hpp"
#include "Bars/TSBarsData.hpp"
#include "Group.hpp"

bool WindowManager::wmDetected;
WindowManager * WindowManager::instance_ = nullptr;
void WindowManager::create(const std::string &displayStr) {
	Logger::GetInstance()->Log("================ Yggdrasil Initialisation ================\n\n", L_INFO);
	if (WindowManager::instance_ != nullptr) {
		throw std::runtime_error("WindowManager instance already created");
	}
	const char *displayCStr =
			displayStr.empty() ? nullptr : displayStr.c_str();
	Display *display = XOpenDisplay(displayCStr);
	if (display == nullptr) {
		Logger::GetInstance()->Log("Failed to open X display " + std::string(XDisplayName(displayCStr)), L_ERROR);
		throw std::runtime_error("Failed to open X display");
	}
	Logger::GetInstance()->Log("Opened X Display:\t" + std::string(XDisplayName(displayCStr)), L_INFO);
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
	Logger::GetInstance()->Log("WindowManager destroyed", L_INFO);
}
void WindowManager::init() {
	selectEventOnRoot();
	ConfigHandler::GetInstance().getConfigData<ConfigDataBindings>()->grabKeys(display_, root_);
	if (wmDetected) {
		throw std::runtime_error("Another window manager is already running.");
	}
	XGrabServer(display_);
	TSBarsData *tsData = new TSBarsData();
	Bars::createInstance();
	getTopLevelWindows();
	Bars::getInstance().init(ConfigHandler::GetInstance().getConfigData<ConfigDataBars>(), tsData, display_, root_);
	Bars::getInstance().start_thread();
	int sizeX = DisplayWidth(display_, DefaultScreen(display_)) - Bars::getInstance().getSpaceE() - Bars::getInstance().getSpaceW() - active_group_->getBorderSize() * 2;
	int sizeY = DisplayHeight(display_, DefaultScreen(display_)) - Bars::getInstance().getSpaceN() - Bars::getInstance().getSpaceS() - active_group_->getBorderSize() * 2;
	int posX = Bars::getInstance().getSpaceW();
	int posY = Bars::getInstance().getSpaceN();
	for (auto g:groups_) {
		g->resize(sizeX,sizeY,posX,posY);
	}
	XUngrabServer(display_);
	XFlush(display_);
	tsData->addData("test", "test");
	signal(SIGINT, handleSIGHUP);
}
void WindowManager::selectEventOnRoot() const {
	XSetErrorHandler(&WindowManager::onWmDetected);
	XSelectInput(
			display_,
			root_,
			SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask);
	XSetErrorHandler(&WindowManager::OnXError);
	XSync(display_, false);
}
void WindowManager::getTopLevelWindows() {
	Window returnedRoot, returnedParent;
	Window *topLevelWindows;
	unsigned int numTopLevelWindows;
	XQueryTree(
			display_,
			root_,
			&returnedRoot,
			&returnedParent,
			&topLevelWindows,
			&numTopLevelWindows);
	if (returnedRoot != root_) {
		throw std::runtime_error("Root window is not the same as the one returned by XQueryTree");
	}
	addGroupsFromConfig();
	Logger::GetInstance()->Log("Found " + std::to_string(numTopLevelWindows) + " top level windows.\troot:" + std::to_string(root_), L_INFO);
	for (unsigned int i = 0; i < numTopLevelWindows; ++i) {
		auto *newClient = new Client(display_, root_, topLevelWindows[i], active_group_, active_group_->getInactiveColor(), active_group_->getBorderSize());
		Client_Err err = newClient->frame();
		setFocus(newClient);
		if (err == YGG_CLI_ERR_RETRIEVE_ATTR) {
			delete newClient;
			Logger::GetInstance()->Log("Failed to frame client: " + std::to_string(topLevelWindows[i]), L_ERROR);
			continue;
		} else if (err == YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT)
		{
			delete newClient;
			Logger::GetInstance()->Log("Ignoring override redirect window: " + std::to_string(topLevelWindows[i]), L_WARNING);
			continue;
		} else if (err == YGG_CLI_LOG_ALREADY_FRAMED) {
			delete newClient;
			Logger::GetInstance()->Log("Client already framed: " + std::to_string(topLevelWindows[i]), L_WARNING);
			continue;
		}
		clients_[newClient->getWindow()] = newClient;
	}
	XFree(topLevelWindows);
}
void WindowManager::addGroupsFromConfig() {
	auto configGroups = ConfigHandler::GetInstance().getConfigData<ConfigDataGroups>()->getGroups();
	for (auto group: configGroups) {
		Group *g = new Group(group);
		groups_.push_back(g);
	}
	groups_[0]->setActive(true);
	active_group_ = groups_[0];
	Logger::GetInstance()->Log("Active Group is [" + active_group_->getName() + "]", L_INFO);
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
	int borderSize = active_group_->getBorderSize();
	unsigned long inactiveColor = active_group_->getInactiveColor();
	auto *client = new Client(display_, root_, window, active_group_, inactiveColor, borderSize);
	Logger::GetInstance()->Log("Inserting client in map: " + client->getTitle() + "\t[" + std::to_string(window) + "]", L_INFO);
	clients_.insert({window, client});
}
void WindowManager::Bar() {
	int screen = DefaultScreen(display_);
//	int barHeight = ConfigHandler::GetInstance().getConfigData<ConfigDataBars>()->getBar(0)->getBarSize();
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
	XClientMessageEvent ev;
	memset(&ev, 0, sizeof(ev));
	ev.type = ClientMessage;
	ev.window = root_;
	ev.message_type = XInternAtom(display_, "WM_PROTOCOLS", False);
	ev.format = 32;
	ev.data.l[0] = XInternAtom(display_, "WM_DELETE_WINDOW", False);
	XSendEvent(display_, root_, False, NoEventMask, (XEvent *)&ev);
	XFlush(display_); // Ensure the event is sent immediately
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
	char errorText[MAX_ERROR_TEXT_LENGTH];
	XGetErrorText(display, e->error_code, errorText, sizeof(errorText));
	std::stringstream errorStream;
	errorStream << "Received X error:\n"
				<< "    Request: " << int(e->request_code)
				<< "    Error code: " << int(e->error_code)
				<< " - " << errorText << "\n"
				<< "    Resource ID: " << e->resourceid;
	Logger::GetInstance()->Log(errorStream.str(), L_ERROR);
	// The return value is ignored.
	return 0;
}
int WindowManager::onWmDetected([[maybe_unused]] Display *display, XErrorEvent *e) {
	if (static_cast<int>(e->error_code) == BadAccess)
		wmDetected = true;
	return 0;
}
