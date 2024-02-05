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
 * @file Client.hpp
 * @brief  This class is responsible for managing the client windows.
 * @date 2024-02-04
 *
 */
#ifndef YGGDRASILWM_CLIENT_HPP
#define YGGDRASILWM_CLIENT_HPP
extern "C" {
#include <X11/Xlib.h>
}

#include <string>
class Group;
class LayoutManager;
/**
 * @enum Client_Err
 * @brief Client_Err enum to handle the error of the Client class
 */
enum Client_Err {
	YGG_CLI_NO_ERROR,
	YGG_CLI_LOG,
	YGG_CLI_LOG_ALREADY_FRAMED,
	YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT,
	YGG_CLI_LOG_IGNORE_NOT_FRAMED,
	YGG_CLI_WARNING,
	YGG_CLI_ERROR,
	YGG_CLI_ERR_RETRIEVE_ATTR,
};
/**
 * @class Client
 * @brief The Client class is responsible for managing the client windows.
 * It creates a frame around the client window, Map the frame, Add the window to the save set, Reparent it, grab the buttons
 * It also unframe the client window by removing the frame and reparenting the window to the root window
 * It also move, resize, restack the client window
 */
class Client {
public:
/**
 * @fn Client(Display *display, Window root, Window window, TreeLayoutManager *layout_manager, unsigned long InActiveColor, int BorderSize)
 * @brief Client constructor.get window class and title
 * @param display X11 display
 * @param root root window
 * @param window window to manage
 * @param layout_manager Reference to the layout manager
 * @param InActiveColor color of the border
 * @param BorderSize size of the border
 */
	Client(Display *display,
		   Window root,
		   Window window,
		   LayoutManager *layout_manager,
		   Group *group,
		   unsigned long InActiveColor,
		   int BorderSize);
/**
 * @fn ~Client()
 * @brief Client destructor Destroy the Client object & the Frame Window it needed
 */
	~Client();
/**
 * @fn Client_Err Client::frame()
 * @brief Client::frame create a frame around the client window, Map the frame,
 * Add the window to the save set Reparent it, grab the buttons
 */
	Client_Err frame();
/**
 * @fn Client_Err Client::unframe()
 * @brief Client::unframe unframe the client window by removing the frame and reparenting the window to the root window
 * @return Client_Err error code from the Client_Err enum
 */
	Client_Err unframe();
/**
 * @fn Window Client::getWindow() const
 * @brief Client::getWindow return the window attribute of the client
 * @return Window
 */
	Window getWindow() const;
/**
 * @fn LayoutManager *Client::getLayoutManager() const
 * @brief return the layout manager of the client
 */
	LayoutManager *getLayoutManager() const;
/**
 * @fn Group *Client::getGroup() const
 * @brief return the group of the client
 * @return
 */
	Group *getGroup() const;
/**
 * @fn static std::string Client::getError(Client_Err error)
 * @brief Client::getError Return a string from an error of Client_Err enum
 * @param error error code from the Client_Err enum
 * @return String of error
 */
	static std::string getError(Client_Err error);
/**
 * @fn void Client::move(int x, int y)
 * @brief: Client::move move the client window to the given position
 * @param x
 * @param y
 */
	void move(int x, int y);
/**
 * @fn void Client::resize(int width, int height)
 * @brief Client::resize resize the client window to the given size
 * @param width
 * @param height
 */
	void resize(int width, int height);
/**
 * @fn bool Client::isFramed() const
 * @brief Client::isFramed() check if the client is framed
 * @return
 */
	bool isFramed() const;
/**
 * @fn void Client::setMapped(bool mapped)
 * @brief Client::setMapped() set the mapped status of the client
 * @param mapped
 * @todo : update Mapped status in EventHandler Class
 */
	void setMapped(bool mapped);
/**
 * @fn bool Client::isMapped() const
 * @brief Client::isMapped() check if the client is mapped
 * @return bool
 */
	bool isMapped() const;
/**
 * @fn const std::string &Client::getTitle() const
 * @brief Client::getTitle() return the title of the client, the title is get during the construction of the client
 * @return std::string
 */
	const std::string &getTitle() const;
/**
 * @fn const std::string &Client::getClass() const
 * @brief Client::getClass() return the class of the client, the class is get during the construction of the client
 * @return
 */
	const std::string &getClass() const;
/**
 * @fn Window Client::getFrame() const
 * @brief Client::getFrame() return the frame Window of the client
 * @return Window (frame)
 */
	Window getFrame() const;
/**
 * @fn void Client::restack()
 * @brief Client::restack() restack the client window to avoid the frame to get in front of the client
 */
	void restack();

private:
	Display *display_;
	LayoutManager *layout_manager_;
	Window root_;
	Window window_;
	Window frame_;
	unsigned int border_width;
	unsigned long border_color;
	bool framed;
	bool mapped{};
	std::string title_;
	std::string class_;
	Group *group_;
};

#endif //YGGDRASILWM_CLIENT_HPP
