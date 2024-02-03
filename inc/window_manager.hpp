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
 * @date 2024-02-03
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
#include "EventHandler.hpp"
#include <algorithm>
#include <csignal>
using ConfigValue = std::variant<std::string, int, bool,unsigned long>;

template <typename T>
struct Size {
	T width, height;

	Size() = default;
	Size(T w, T h)
			: width(w), height(h) {
	}
};
template <typename T>
::std::ostream& operator << (::std::ostream& out, const Size<T>& size);
template <typename T>
struct Position {
	T x, y;

	Position() = default;
	Position(T _x, T _y)
			: x(_x), y(_y) {
	}

};
/**
 * @Brief WindowManager class
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
 * @brief Initialize the WindowManager
 * set the event select mask on the root window
 * creates clients for the existing top level windows
 * and launch the bar window
 */
	void Init();
/**
 * @brief Run the window manager
 */
	void Run();
// Getters
/**
 * @brief Get the Logger object
 */
	const Logger &getLogger() const;
/**
 * @brief Get the Display object
 */
	Display *getDisplay() const;
/**
 * @brief Get the Root Window pointer
 */
	const Window getRoot() const;
/**
 * @brief Get the Layout Manager object
 */
	TreeLayoutManager *getLayoutManager() const;
/**
 * @brief Get the Clients map
 */
	std::unordered_map<Window, Client *> &getClients();
/**
 * @brief Get the Client by window ptr or frame ptr
 * @param window
 * @return nullptr if not found
 */
	Client * getClient(Window window);
/**
 * @brief Get a ref to the Client by window ptr does not look for frames and throws if not found
 */
	Client &getClientRef(Window window);
/**
 * @brief check if a window is a frame
 */
	bool isFrame(Window window);
/**
 * @brief Get the Bar window
 */
	Window getBar() const;
/**
 * @brief Get the number of clients
 */
	int getClientCount();
// Setters
/**
 * @brief Set the focus to a client
 */
	void setFocus(Client *client);
/**
 * @brief Insert a client in the clients map
 * @param window
 */
	void insertClient(Window window);
// Running control
/**
 * @brief Stop the window manager
  TODO: implement a clean way to stop the window manager
 */
	void Stop();
/**
 * @brief Get the Running status
 */
	bool getRunning() const;
/**
 * @brief Get the Config Handler object
 */
	ConfigHandler &getConfigHandler();

private:
	Display									*display_;
	static bool								wm_detected_;
	const Window							root_;
	Window 									bar_;
	const Logger&							logger_;
	ConfigHandler							configHandler_;
	TreeLayoutManager						*layout_manager_;
	const Atom								WM_PROTOCOLS;
	const Atom								WM_DELETE_WINDOW;
	bool									running;
	std::unordered_map<Window, Client*>		clients_;
// Initialisation
/**
 * @brief Construct a new WindowManager object
 */
	WindowManager(Display *display, const Logger &logger,ConfigHandler &configHandler);
/**
 * @brief set the event mask on the root window and register as the window manager for the X session
 */
	void selectEventOnRoot() const;
/**
 * @brief look for existing top level windows and create clients for them
 */
	void getTopLevelWindows(std::stringstream &debug_stream);
/**
 * @brief create the bar window
 */
	void Bar();
// Error Management
/**
 * @brief X11 Error handler
 */
	static int OnXError(Display *display, XErrorEvent *e);
/**
 * @brief When selecting events on the root window, we may get a BadAccess error if another window manager is running
  this function is called when a BadAccess error is detected
  it sets the wm_detected_ flag to true and stops the window manager
  only one window manager can run at a time
 */
	static int OnWMDetected(Display *display, XErrorEvent *e);
};
#endif