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
 * @file MasterLayoutManager.cpp
 * @brief handle Master Style Layout.
 * @date 2024-03-21
 */

#include "Layouts/MasterLayoutManager.hpp"
#include "Layouts/MasterSpace.hpp"

void MasterLayoutManager::removeClient(Client *client) {

}

void MasterLayoutManager::addClient(std::shared_ptr<Client> client) {

}

void MasterLayoutManager::reSize(const Point &size, const Point &pos) {

}

void MasterLayoutManager::updateGeometry(unsigned int sizeX, unsigned int sizeY, unsigned int posX, unsigned int posY) {

}

MasterLayoutManager::MasterLayoutManager(Display *display,
										 Window root,
										 int sizeX,
										 int sizeY,
										 int posX,
										 int posY,
										 int borderSize,
										 int gap,
										 int barHeight) :
		LayoutManager(display,root,sizeX,sizeY,posX,posY,gap,barHeight) {
	Point pos(posX, posY);
	Point size(sizeX - borderSize, sizeY - borderSize);
	this->rootSpace_ = std::make_unique<MasterSpace>(pos, size,0, true, true);
}
