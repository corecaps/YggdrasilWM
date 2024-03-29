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
extern "C" {
#include <X11/Xlib.h>
}
#include "WindowManager.hpp"
#include "EventHandler.hpp"
#include "Config/ConfigDataBars.hpp"
#include "Config/ConfigHandler.hpp"
#include "Config/ConfigDataGroups.hpp"
#include "Config/ConfigDataBindings.hpp"
#include "Bars/Bars.hpp"
#include "Bars/TSBarsData.hpp"
#include "Group.hpp"
#include "Ewmh.hpp"
#include "X11wrapper/baseX11Wrapper.hpp"
#include "YggdrasilExceptions.hpp"
bool WindowManager::wmDetected;
WindowManager * WindowManager::instance_ = nullptr;
void WindowManager::create(std::shared_ptr<BaseX11Wrapper> wrapper,const std::string &displayStr) {
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
	WindowManager::instance_ = new WindowManager(display,wrapper);
}
WindowManager::WindowManager(Display *display, const std::shared_ptr<BaseX11Wrapper>& wrapper)
		: display_(display),
		  root_(DefaultRootWindow(display)),
		  WM_PROTOCOLS( wrapper->internAtom(display_, "WM_PROTOCOLS", false)),
		  WM_DELETE_WINDOW(wrapper->internAtom(display_, "WM_DELETE_WINDOW", false)),
		  running(true),
		  tsData(nullptr),
		  geometryX(0),
		  geometryY(0),
		  activeWindow(0),
		  x11Wrapper(wrapper){}
WindowManager::~WindowManager() {
	clients_.clear();
	groups_.clear();
	Logger::GetInstance()->Log("WindowManager destroyed", L_INFO);
}
void WindowManager::init() {
	selectEventOnRoot();
	ConfigHandler::GetInstance().getConfigData<ConfigDataBindings>()->initKeycodes(display_,x11Wrapper.get());
	if (wmDetected) {
		throw std::runtime_error("Another window manager is already running.");
	}
	ConfigHandler::GetInstance().getConfigData<ConfigDataBindings>()->grabKeys(display_, root_);
	geometryX = x11Wrapper->displayWidth(display_, x11Wrapper->defaultScreen(display_));
	geometryY = x11Wrapper->displayHeight(display_, x11Wrapper->defaultScreen(display_));
	x11Wrapper->grabServer(display_);
	ewmh::initEwmh(display_,root_);
	tsData = std::make_shared<TSBarsData>();
	getTopLevelWindows();
	createBars();
	x11Wrapper->ungrabServer(display_);
	ewmh::updateWmProperties(display_, root_);
	x11Wrapper->flush(display_);
	tsData->addData("EvCount", "0");
	signal(SIGINT, handleSIGHUP);
}
void WindowManager::selectEventOnRoot() const {
	x11Wrapper->setErrorHandler(&WindowManager::onWmDetected);
	x11Wrapper->selectInput(
			display_,
			root_,
			SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask | ClientMessage);
	x11Wrapper->setErrorHandler(&WindowManager::OnXError);
	x11Wrapper->sync(display_, false);
}
void WindowManager::getTopLevelWindows() {
	Window returnedRoot, returnedParent;
	Window *topLevelWindows;
	unsigned int numTopLevelWindows;
	x11Wrapper->queryTree(
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
		std::shared_ptr<Client> newClient = nullptr;
		try {
			auto g = getActiveGroup();
			newClient = std::make_shared<Client>(display_, root_, topLevelWindows[i], g,
										 g->getInactiveColor(), g->getBorderSize(), x11Wrapper);
			newClient->frame();
			g->addClient(newClient->getWindow(),newClient);
			setFocus(newClient.get());
		} catch (const YggdrasilException &e) {
			Logger::GetInstance()->Log(e.what(), L_ERROR);
			continue;
		}
		if (newClient != nullptr) {
			clients_[newClient->getWindow()] = newClient;
		}
	}
	x11Wrapper->freeX(topLevelWindows);
}
void WindowManager::createBars() {
	Bars::createInstance();
	Bars::getInstance().init(ConfigHandler::GetInstance().getConfigData<ConfigDataBars>(), tsData, display_, root_);
	Bars::getInstance().start_thread();
	auto g = getActiveGroup();
	unsigned int sizeX = geometryX- Bars::getInstance().getSpaceE() - Bars::getInstance().getSpaceW() - g->getBorderSize() * 2;
	unsigned int sizeY = geometryY - Bars::getInstance().getSpaceN() - Bars::getInstance().getSpaceS() - g->getBorderSize() * 2;
	unsigned int posX = Bars::getInstance().getSpaceW();
	unsigned int posY = Bars::getInstance().getSpaceN();
	for (auto &g:groups_) {
		g->resize(sizeX,sizeY,posX,posY);
	}
}
void WindowManager::addGroupsFromConfig() {
	auto configGroups = ConfigHandler::GetInstance().getConfigData<ConfigDataGroups>()->getGroups();
	std::string groupsNames = "";
	for (auto group: configGroups) {
		std::shared_ptr<Group> g = std::make_shared<Group>(group, x11Wrapper,display_,root_);
		groupsNames += g->getName() + ",";
		groups_.push_back(g);
	}
	if (groupsNames.back() == ',') {
		groupsNames.pop_back();
	}
	tsData->addData("Groups", groupsNames);
	groups_[0]->setActive(true);
	tsData->addData("ActiveGroup", groups_[0]->getName());
	active_group_ = groups_[0];
	Logger::GetInstance()->Log("Active Group is [" + getActiveGroup()->getName() + "]", L_INFO);
}
void WindowManager::Run() {
	Logger::GetInstance()->Log("================ Yggdrasil WM Running ================\n\n", L_INFO);
	EventHandler::create();
	XEvent e;
//	int evcount = 0;
	while (running && !x11Wrapper->nextEvent(display_, &e)) {
		EventHandler::getInstance()->dispatchEvent(e);
		x11Wrapper->sync(display_, false);
//		tsData->modifyData("EvCount", std::to_string(evcount++));
	}
	Logger::GetInstance()->Log("WindowManager stopped", L_INFO);
//	XCloseDisplay(display_);
}
void WindowManager::testRun() {
	try {
		EventHandler::getInstance();
	} catch (const std::exception &e) {
		EventHandler::create();
	}
	XSync(display_, false);
	XEvent ev;
	x11Wrapper->nextEvent(display_, &ev);
	EventHandler::getInstance()->dispatchEvent(ev);
	x11Wrapper->sync(display_, false);
}
void WindowManager::insertClient(Window window) {
	try {
		int borderSize = getActiveGroup()->getBorderSize();
		unsigned long inactiveColor = getActiveGroup()->getInactiveColor();
		std::shared_ptr<Client> client = std::make_shared<Client>(display_, root_, window, getActiveGroup(), inactiveColor,
																  borderSize, x11Wrapper);
		Logger::GetInstance()->Log(
				"Inserting client in map: " + client->getTitle() + "\t[" + std::to_string(window) + "]", L_INFO);
		clients_.insert({window, client});
	} catch (const YggdrasilException &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
void WindowManager::setFocus(Client *client) {
	if (client != nullptr) {
		x11Wrapper->setInputFocus(display_, client->getWindow(), RevertToParent, CurrentTime);
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
	ev.data.l[0] = static_cast<long>(XInternAtom(display_, "WM_DELETE_WINDOW", False));
	try {
		x11Wrapper->sendEvent(display_, root_, False, NoEventMask, (XEvent *) &ev);
		x11Wrapper->flush(display_); // Ensure the event is sent immediately
	} catch (const YggdrasilException &e) {
		std::cerr << e.what();
	}
	Bars::getInstance().stop_thread();
	Bars::destroy();
	std::cout << "Stopping WindowManager" << std::endl;
}
WindowManager * WindowManager::getInstance() {
	if (WindowManager::instance_ != nullptr) {
		return WindowManager::instance_;
	} else {
		throw std::runtime_error("WindowManager instance not created");
	}
}
std::shared_ptr<Client> WindowManager::getClient(Window window) {
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
std::unordered_map<Window, std::shared_ptr<Client>> & WindowManager::getClients() { return clients_; }
std::shared_ptr<Client> WindowManager::getClientRef(Window window) { return clients_.at(window); }
Window WindowManager::getRoot() const { return root_; }
unsigned long WindowManager::getClientCount() { return clients_.size(); }
void WindowManager::setActiveGroup(std::shared_ptr<Group> activeGroup) {
	tsData->modifyData("ActiveGroup", activeGroup->getName());
	active_group_ = std::weak_ptr<Group> (activeGroup);
}
std::shared_ptr <Group>WindowManager::getActiveGroup() const {
	auto g = active_group_.lock();
	if (g)
		return g;
	else
		throw YggdrasilException("invalid active group");
}
const std::vector<std::shared_ptr<Group>> & WindowManager::getGroups() const { return groups_; }
bool WindowManager::getRunning() const { return running; }
unsigned int WindowManager::getGeometryX() const { return geometryX; }
unsigned int WindowManager::getGeometryY() const { return geometryY; }
Window WindowManager::getActiveWindow() const { return activeWindow; }
const std::shared_ptr<BaseX11Wrapper> &WindowManager::getX11Wrapper() const { return x11Wrapper; }
void WindowManager::setActiveWindow(Window aWindow) { WindowManager::activeWindow = aWindow; }
int WindowManager::OnXError(Display *display, XErrorEvent *e) {
	const int MAX_ERROR_TEXT_LENGTH = 1024;
	char errorText[MAX_ERROR_TEXT_LENGTH];
	WindowManager::getInstance()->getX11Wrapper()->getErrorText(display, e->error_code, errorText, sizeof(errorText));
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

void WindowManager::setActiveGroup(Group *activeGroup) {
	WindowManager::active_group_ = std::shared_ptr<Group>(activeGroup);
}




