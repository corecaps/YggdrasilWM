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
 * @file Group.cpp
 * @brief Group class implementation.
 * @date 2024-02-12
 */
#include "WindowManager.hpp"
#include "Group.hpp"

#include <utility>
#include "Client.hpp"
#include "Layouts/LayoutManager.hpp"

Group::Group(ConfigDataGroup *config) {
	name_ = config->getGroupName();
	LayoutType layoutType;
	if (config->getGroupLayout() == "Tree") {
		layoutType = TREE;
	} else if (config->getGroupLayout() == "Max") {
		layoutType = MAX;
	} else if (config->getGroupLayout() == "Vertical") {
		layoutType = VERTICAL;
	} else if (config->getGroupLayout() == "Horizontal") {
		layoutType = HORIZONTAL;
	} else {
		layoutType = TREE;
	}
	borderSize_ = config->getGroupBorderWidth();
	gap_ = config->getGroupGap();
	inactiveColor_ = config->getGroupInactiveColor();
	activeColor_ = config->getGroupActiveColor();
	barHeight_ = 30;
	active_ = false;
	Logger::GetInstance()->Log("Group Created [" + name_ + "]", L_INFO);
	Display *display = WindowManager::getInstance()->getDisplay();
	int size_x = DisplayWidth(display, DefaultScreen(display));
	int size_y = DisplayHeight(display, DefaultScreen(display));
	switch (layoutType) {
		case TREE:
			layoutManager_ = new TreeLayoutManager(display,
												   WindowManager::getInstance()->getRoot(),
												   size_x,
												   size_y,
												   0,
												   0,
												   borderSize_,
												   gap_,
												   barHeight_);
			break;
		case MAX:
		case VERTICAL:
		case HORIZONTAL:
		default:
			layoutManager_ = new TreeLayoutManager(display,
												   WindowManager::getInstance()->getRoot(),
												   size_x,
												   size_y,
												   0,
												   0,
												   borderSize_,
												   gap_,
												   barHeight_);
			break;
	}
}
Group::~Group() {
	delete layoutManager_;
}
void Group::AddClient(Window window, Client *client) {
	clients_[window] = client;
	layoutManager_->addClient(client);
}
void Group::RemoveClient(Window window) {
	clients_.erase(window);
	try {
		auto c = WindowManager::getInstance()->getClient(window);
		layoutManager_->removeClient(c);
	} catch (const std::exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
void Group::RemoveClient(Client *client) {
	clients_.erase(client->getWindow());
	layoutManager_->removeClient(client);
}
void Group::SetActive(bool active) { active_ = active; }
void Group::moveClientToGroup(Window window, Group *group) {
	clients_.erase(window);
	try {
		auto c = WindowManager::getInstance()->getClient(window);
		group->AddClient(window, c);
	} catch (const std::exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
void Group::switchTo() {
	Logger::GetInstance()->Log("Group switched to [" + name_ + "]", L_INFO);
	for (auto &client: WindowManager::getInstance()->getClients()) {
		if (client.second->getGroup() == this) {
			XMapWindow(WindowManager::getInstance()->getDisplay(), client.second->getFrame());
		}
	}
	this->active_= true;
	WindowManager::getInstance()->setActiveGroup(this);
}
void Group::switchFrom() {
	Logger::GetInstance()->Log("Group switched from [" + name_ + "]", L_INFO);
	for (auto &client: WindowManager::getInstance()->getClients()) {
		if (client.second->getGroup() == this) {
//			client.second->unframe();
			XUnmapWindow(WindowManager::getInstance()->getDisplay(), client.second->getFrame());
		}
	}
	this->active_ = false;
}
void Group::SetName(std::string name) {
	name_ = std::move(name);
}
bool Group::IsActive() const { return active_; }
std::string Group::GetName() { return name_; }
Client *Group::GetClient(Window window) { return clients_[window]; }
std::unordered_map<Window, Client *> Group::GetClients() { return clients_; }
LayoutManager *Group::GetLayoutManager() { return layoutManager_; }
int Group::getBorderSize() const { return borderSize_; }
int Group::getGap() const { return gap_; }
unsigned long Group::getInactiveColor() const { return inactiveColor_; }
unsigned long Group::getActiveColor() const { return activeColor_; }
