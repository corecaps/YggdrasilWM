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
 * @date 2024-03-21
 */
#include "WindowManager.hpp"
#include "Group.hpp"
#include "X11wrapper/baseX11Wrapper.hpp"
#include <utility>
#include "Client.hpp"
#include "Layouts/LayoutManager.hpp"
#include "Config/ConfigDataBindings.hpp"

Group::Group(const std::shared_ptr<ConfigDataGroup>& config,
			 std::shared_ptr<BaseX11Wrapper> x11Wrapper,
			 Display *display,
			 Window root) {
	name_ = config->getGroupName();
	LayoutType layoutType;
	if (config->getGroupLayout() == "Tree") {
		layoutType = TREE;
	} else if (config->getGroupLayout() == "Max") {
		layoutType = MAX;
	} else if (config->getGroupLayout() == "MasterVertical") {
		layoutType = MASTER_VERTICAL;
	} else if (config->getGroupLayout() == "MasterHorizontal") {
		layoutType = MASTER_HORIZONTAL;
	} else {
		layoutType = TREE;
	}
	wrapper = x11Wrapper;
	borderSize_ = config->getGroupBorderWidth();
	gap_ = config->getGroupGap();
	inactiveColor_ = config->getGroupInactiveColor();
	activeColor_ = config->getGroupActiveColor();
	barHeight_ = 30;
	active_ = false;
	Logger::GetInstance()->Log("Group Created [" + name_ + "]", L_INFO);
	int size_x = wrapper->displayWidth(display, wrapper->defaultScreen(display));
	int size_y = wrapper->displayHeight(display, wrapper->defaultScreen(display));
	switch (layoutType) {
		case TREE:
			layoutManager_ = std::make_shared<TreeLayoutManager>(display,
												   root,
												   size_x,
												   size_y,
												   0,
												   0,
												   borderSize_,
												   gap_,
												   barHeight_);
			break;
		case MAX:
			break;
		case MASTER_VERTICAL:
			layoutManager_ = std::make_shared<TreeLayoutManager>(display,
												   root,
												   size_x,
												   size_y,
												   0,
												   0,
												   borderSize_,
												   gap_,
												   barHeight_);
			break;
		case MASTER_HORIZONTAL:
			layoutManager_ = std::make_shared<TreeLayoutManager>(display,
												   root,
												   size_x,
												   size_y,
												   0,
												   0,
												   borderSize_,
												   gap_,
												   barHeight_);
			break;
		default:
			layoutManager_ = std::make_shared<TreeLayoutManager>(display,
												   root,
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
Group::~Group() { }
void Group::addClient(Window window,std::shared_ptr<Client> client) {
	clients_[window] = client;
	layoutManager_->addClient(client);
}
void Group::removeClient(Window window) {
	clients_.erase(window);
	try {
		auto c = WindowManager::getInstance()->getClient(window);
		layoutManager_->removeClient(c.get());
	} catch (const std::exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
void Group::removeClient(Client *client) {
	clients_.erase(client->getWindow());
	layoutManager_->removeClient(client);
}
void Group::setActive(bool active) { active_ = active; }
void Group::moveClientToGroup(Window window, Group *group) {
	clients_.erase(window);
	try {
		auto c = WindowManager::getInstance()->getClient(window);
		group->addClient(window, c);
	} catch (const std::exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
void Group::switchTo() {
	Logger::GetInstance()->Log("Group switched to [" + name_ + "]", L_INFO);
	for (auto &client: WindowManager::getInstance()->getClients()) {
		if (client.second->getGroup().get() == this) {
			wrapper->mapWindow(WindowManager::getInstance()->getDisplay(), client.second->getFrame());
		}
	}
	this->active_= true;
	WindowManager::getInstance()->setActiveGroup(shared_from_this());
}
void Group::switchFrom() {
	Logger::GetInstance()->Log("Group switched from [" + name_ + "]", L_INFO);
	for (auto &client: WindowManager::getInstance()->getClients()) {
		if (client.second->getGroup().get() == this) {
//			client.second->unframe();
			wrapper->unmapWindow(WindowManager::getInstance()->getDisplay(), client.second->getFrame());
		}
	}
	this->active_ = false;
}
void Group::setName(std::string name) {
	name_ = std::move(name);
}
bool Group::isActive() const { return active_; }
std::string Group::getName() { return name_; }
std::shared_ptr<Client> Group::getClient(Window window) { return clients_[window]; }
std::unordered_map<Window, std::shared_ptr<Client>> Group::getClients() { return clients_; }
std::shared_ptr <LayoutManager> Group::getLayoutManager() { return layoutManager_; }
int Group::getBorderSize() const { return borderSize_; }
int Group::getGap() const { return gap_; }
unsigned long Group::getInactiveColor() const { return inactiveColor_; }
unsigned long Group::getActiveColor() const { return activeColor_; }

void Group::resize(unsigned int sizeX, unsigned int sizeY, unsigned int posX, unsigned int posY) {
	layoutManager_->updateGeometry(sizeX, sizeY, posX, posY);
}
