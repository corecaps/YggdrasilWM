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
 * @file window_manager.hpp
 * @brief WindowManager class header.
 * @date 2024-02-05
 */

#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP
extern "C" {
#include <X11/Xlib.h>
}
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include "Logger.hpp"
#include "Client.hpp"
#include "TreeLayoutManager.hpp"
#include "ConfigHandler.hpp"
#include <iostream>
#include <algorithm>
#include <csignal>
#include "Group.hpp"
using ConfigValue = std::variant<std::string, int, bool,unsigned long>;

/**
 * @class WindowManager
 * @brief WindowManager class
 * This class is responsible for managing the windows.in an X11 session.
 * It is the main class of the project.
 * It should be instantiated only via the static Create function.
 * You must use the Init function to initialize the WindowManager.
 * The Run function is the main loop of the WindowManager.
 */
class WindowManager {
public:
/**
 * @brief Create a WindowManager object
 * @param logger
 * @param configHandler
 * @param display_str
 * @return
 */
	static ::std::unique_ptr<WindowManager> Create(
		Logger &logger,
		ConfigHandler &configHandler,
		const std::string &display_str = std::string());
/**
 * @brief Destroy the WindowManager object
 * i have yet to find a clean way to close the window manager
 */
	~WindowManager();
/**
 * @fn void WindowManager::Init()
 * @brief Initialize the WindowManager
 * set the event select mask on the root window
 * creates clients for the existing top level windows
 * and launch the bar window
 */
	void Init();
/**
 * @fn void WindowManager::Run()
 * @brief Run the window manager
 */
	void Run();
// Getters
/**
 * @fn const Logger &WindowManager::getLogger() const
 * @brief Get the Logger object
 */
	const Logger &getLogger() const;
/**
 * @fn Display *WindowManager::getDisplay() const
 * @brief Get the Display object
 */
	Display *getDisplay() const;
/**
 * @fn Window WindowManager::getRoot() const
 * @brief Get the Root Window pointer
 */
	Window getRoot() const;
/**
 * @fn std::unordered_map<Window, Client *> &WindowManager::getClients()
 * @brief Get the Clients map
 */
	std::unordered_map<Window, Client *> &getClients();
/**
 * @fn Client *WindowManager::getClient(Window window)
 * @brief Get the Client by window ptr or frame ptr
 * @param window
 * @return nullptr if not found
 */
	Client * getClient(Window window);
/**
 * @fn Client &WindowManager::getClientRef(Window window)
 * @brief Get a ref to the Client by window ptr does not look for frames and throws if not found
 */
	Client &getClientRef(Window window);
/**
 * @fn bool WindowManager::isFrame(Window window)
 * @brief check if a window is a frame
 */
	bool isFrame(Window window);
/**
 * @fn Window WindowManager::getBar() const
 * @brief Get the Bar window
 */
	Window getBar() const;
/**
 * @fn unsigned long WindowManager::getClientCount()
 * @brief Get the number of clients
 */
	unsigned long getClientCount();
// Setters
/**
 * @fn void WindowManager::setFocus(Client *client)
 * @brief Set the focus to a client
 */
	void setFocus(Client *client);
/**
 * @fn void WindowManager::insertClient(Window window)
 * @brief Insert a client in the clients map
 * @param window
 */
	void insertClient(Window window);
// Running control
/**
 * @fn void WindowManager::Stop()
 * @brief Stop the window manager
  @todo: implement a clean way to stop the window manager
 */
	void Stop();
/**
 * @fn bool WindowManager::getRunning() const
 * @brief Get the Running status
 */
	bool getRunning() const;
/**
 * @fn ConfigHandler &WindowManager::getConfigHandler()
 * @brief Get the Config Handler object
 */
	ConfigHandler &getConfigHandler();

private:
	Display									*display_;
	static bool								wm_detected_;
	const Window							root_;
	Window 									bar_{};
	const Logger&							logger_;
	ConfigHandler							configHandler_;
	std::vector<Group>						groups_;
	const Atom								WM_PROTOCOLS;
	const Atom								WM_DELETE_WINDOW;
	bool									running;
	std::unordered_map<Window, Client*>		clients_;
// Initialisation
/**
 * @fn WindowManager::WindowManager(Display *display, const Logger &logger,ConfigHandler &configHandler)
 * @brief Construct a new WindowManager object
 */
	WindowManager(Display *display, const Logger &logger,ConfigHandler &configHandler);
/**
 * @fn void WindowManager::selectEventOnRoot() const
 * @brief set the event mask on the root window and register as the window manager for the X session
 */
	void selectEventOnRoot() const;
/**
 * @fn void WindowManager::getTopLevelWindows(std::stringstream &debug_stream)
 * @brief look for existing top level windows and create clients for them
 */
	void getTopLevelWindows(std::stringstream &debug_stream);
/**
 * @fn void WindowManager::Bar()
 * @brief create the bar window
 */
	void Bar();
// Error Management
/**
 * @fn static int WindowManager::OnXError(Display *display, XErrorEvent *e)
 * @brief X11 Error handler
 */
	static int OnXError(Display *display, XErrorEvent *e);
/**
 * @fn static int WindowManager::OnWMDetected(Display *display, XErrorEvent *e)
 * @brief When selecting events on the root window, we may get a BadAccess error if another window manager is running
  this function is called when a BadAccess error is detected
  it sets the wm_detected_ flag to true and stops the window manager
  only one window manager can run at a time
 */
	static int OnWMDetected([[maybe_unused]] Display *display, XErrorEvent *e);
};
#endif