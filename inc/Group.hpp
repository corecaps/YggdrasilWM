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
 * @file Group.hpp
 * @brief  Group class header.
 * @date 2024-02-10
 *
 */

#ifndef YGGDRASILWM_GROUP_H
#define YGGDRASILWM_GROUP_H
#include <unordered_map>
#include <string>
extern "C" {
#include <X11/Xlib.h>
}
#include "Config/ConfigDataGroup.hpp"
class LayoutManager;
class Client;
class WindowManager;
/**
 * @enum LayoutType
 * @brief LayoutType enum
 * This enum is used to define the different layout managers.
 */
enum LayoutType {
	TREE,
	MAX,
	VERTICAL,
	HORIZONTAL,
	FLOAT
};
/**
 * @class Group
 * @brief Groups are defined in the config file.
 * Each group use a specific layout manager and manage it's list of clients,
 * a client can be in multiple groups.
 * @todo Rules on the WM_CLASS of the client can be defined in the config file.
 */
class Group {
public:
	/**
	 * @fn Group(std::string name, LayoutManager* layout_manager)
	 * @brief Group constructor
	 * @param name The name of the group
	 * @param borderSize The size of the border
	 * @param gap The gap between the clients
	 * @param barHeight The height of the bar
	 * @param layoutType The layout type
	 * @todo add the other layout types
	 */
	Group(ConfigDataGroup *config);
/**
 * @fn ~Group()
 * @brief Group destructor
 * @todo implementation
 */
	~Group();
/**
 * @fn void Group::AddClient(Window window, Client *client)
 * @brief Add a client to the group
 * @param window
 * @param client
 */
	void		AddClient(Window window, Client *client);
/**
 * @fn void Group::RemoveClient(Window window)
 * @brief Definitely remove a client from the group
 * @param window
 */
	void		RemoveClient(Window window);
/**
 * @fn void Group::RemoveClient(Client *client)
 * @brief Definitely remove a client from the group
 * @param client
 */
	void		RemoveClient(Client *client);
/**
 * @fn void Group::moveClientToGroup(Window window, Group *group)
 * @brief Move a client to another group
 * @param window
 * @param group
 */
	void		moveClientToGroup(Window window, Group *group);
/**
 * @fn void Group::SetActive(bool active)
 * @brief Set the group as active
 * @param active
 */
	void		SetActive(bool active);
/**
 * @fn bool Group::IsActive()
 * @brief Check if the group is active
 * @return bool
 */
	bool		IsActive() const;
/**
 * @fn void Group::SetName(std::string name)
 * @brief Set the name of the group
 * @param name
 */
	void		SetName(std::string name);
/**
 * @fn std::string Group::GetName()
 * @brief Get the name of the group
 * @return
 */
	std::string	GetName();
/**
 * @fn Client* Group::GetClient(Window window)
 * @brief Get a client from the group
 * @param window
 */
	Client *	GetClient(Window window);
/**
 * @fn std::unordered_map<Window, Client *> GetClients()
 * @brief Get the list of clients of the group
 */
	std::unordered_map<Window, Client *> GetClients();
/**
 * @fn LayoutManager* Group::GetLayoutManager()
 * @brief Get the layout manager of the group
 */
	LayoutManager*	GetLayoutManager();
/**
 * @fn void Group::switchTo()
 * @brief Switch to this group
 * Set this group as active, switch the layout manager and map the clients
 * @todo implementation
 */
	void		switchTo();
/**
 * @fn void Group::switchFrom()
 * @brief Switch from this group
 * Set this group as inactive, switch the layout manager and unmap the clients
 * @todo implementation
 */
	void		switchFrom();
/**
 * @fn int Group::getBorderSize()
 * @brief Get the border size of the group
 * @return
 */
	int			getBorderSize() const;
/**
 * @fn int Group::getGap()
 * @brief Get the gap of the group
 * @return
 */
	int			getGap() const;
/**
 * @fn unsigned long Group::getInactiveColor()
 * @brief Get the inactive color of the group
 * @return
 */
	unsigned long	getInactiveColor() const;
/**
 * @fn unsigned long Group::getActiveColor()
 * @brief Get the active color of the group
 * @return
 */
	unsigned long	getActiveColor() const;

private:
	std::string								name_;
	std::unordered_map<Window, Client *>	clients_;
	LayoutManager*							layoutManager_;
	int										borderSize_;
	int										gap_;
	unsigned long 							inactiveColor_;
	unsigned long 							activeColor_;
	int										barHeight_;
	bool									active_{};
};
#endif //YGGDRASILWM_GROUP_H
