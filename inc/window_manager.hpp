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
 * @date 2021-06-23
 * This class is responsible for managing the windows.in an X11 session.
 * It is the main class of the project.
 * It should be instantiated only via the static Create function.
 * The Run function is the main loop of the program.
 * @see Create
 * @see Run
 * @see Client
 * @see Logger
 * @see EventHandler
 * @see ConfigHandler
 * @see LayoutManager
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

class WindowManager {
public:
	static ::std::unique_ptr<WindowManager> Create(
		Logger &logger,
		ConfigHandler &configHandler,
		const std::string &display_str = std::string());
	~WindowManager();
	void Init();
	void Run();
// For testing
//	void Run(std::chrono::duration<int64_t> duration);
// Getters
	const Logger &getLogger() const;
	Display *getDisplay() const;
	const Window getRoot() const;
	TreeLayoutManager *getLayoutManager() const;
	std::unordered_map<Window, Client *> &getClients();
	Client * getClient(Window window);
	bool isFrame(Window window);
	Client &getClientRef(Window window);
	Window getBar() const;
	int getClientCount();
// Setters
	void setFocus(Client *client);
	void insertClient(Window window);
// Running control
	void Stop();
	bool getRunning() const;

private:
	Display									*display_;
	static bool								wm_detected_;
	const Window							root_;
	Window 									bar_;
public:
	ConfigHandler &getConfigHandler();

private:
	const Logger&							logger_;
	ConfigHandler                    configHandler_;
	TreeLayoutManager						*layout_manager_;
	const Atom								WM_PROTOCOLS;
	const Atom								WM_DELETE_WINDOW;
	bool									running;
	std::unordered_map<Window, Client*>		clients_;
// Initialisation
	WindowManager(Display *display, const Logger &logger,ConfigHandler &configHandler);
	void selectEventOnRoot() const;
	void getTopLevelWindows(std::stringstream &debug_stream);
	void Bar();
	// Error Management
	static int OnXError(Display *display, XErrorEvent *e);
	static int OnWMDetected(Display *display, XErrorEvent *e);
};

#endif