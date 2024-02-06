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
 * @file TreeLayoutManager
 * @brief handle Tree Style Layout.
 * @date 2024-02-06
 */
#include "TreeLayoutManager.hpp"
TreeLayoutManager::TreeLayoutManager(Display *display,
									 Window root,
									 int size_x,
									 int size_y,
									 int pos_x,
									 int pos_y,
									 int borderSize,
									 int gap,
									 int barHeight) :
	LayoutManager(display, root, size_x, size_y, pos_x, pos_y, gap, borderSize, barHeight) {
	Point pos(pos_x, pos_y + barHeight);
	Point size(size_x - borderSize, size_y - barHeight - borderSize);
	this->rootSpace_ = new Space(pos, size, 0);
}
TreeLayoutManager::~TreeLayoutManager() { delete rootSpace_; }
void TreeLayoutManager::updateGeometry() {  }
LayoutManager::Space * TreeLayoutManager::findSpaceRecursive(Client *client, LayoutManager::Space * space) {
	if (space->getClient() == client) {
		return space;
	}
	if (space->getLeft() != nullptr) {
		LayoutManager::Space * leftSpace = findSpaceRecursive(client, space->getLeft().get());
		if (leftSpace != nullptr) {
			return leftSpace;
		}
	}
	if (space->getRight() != nullptr) {
		LayoutManager::Space * rightSpace = findSpaceRecursive(client, space->getRight().get());
		if (rightSpace != nullptr) {
			return rightSpace;
		}
	}
	return nullptr;
}
LayoutManager::Space * TreeLayoutManager::findSpace(Client *client) {
	Space* space = rootSpace_;
	return findSpaceRecursive(client, space);
}
LayoutManager::Space * TreeLayoutManager::findSpace(int index) {
	return nullptr;
}
void TreeLayoutManager::removeClient(Client* client) {
	removeClientRecursive(client, rootSpace_);
}
void TreeLayoutManager::removeClientRecursive(Client* client, Space* space) {
	if (space->getClient() == client) {
		space->setClient(nullptr);
		if (space != rootSpace_) {
			bool isLeftChild = (space->getParent()->getLeft().get() == space);
			Space* siblingSpace = isLeftChild ? space->getParent()->getRight().get() : space->getParent()->getLeft().get();
			if (siblingSpace->getClient() != nullptr) {
				placeClientInSpace(siblingSpace->getClient(), space->getParent());
				siblingSpace->setClient(nullptr);
			}
			if (isLeftChild) {
				space->getParent()->setLeft(nullptr);
			} else {
				space->getParent()->setRight(nullptr);
			}
		}
		return;
	}
	if (space->getLeft() != nullptr) {
		removeClientRecursive(client, space->getLeft().get());
	}
	if (space->getRight() != nullptr) {
		removeClientRecursive(client, space->getRight().get());
	}
}
void TreeLayoutManager::addClient(Client* client) {
	addClientRecursive(client, rootSpace_);
}
void TreeLayoutManager::addClientRecursive(Client* client, Space* space) {
	if (space->getClient() == nullptr && space->getLeft() == nullptr && space->getRight() == nullptr ) {
		placeClientInSpace(client, space);
		return;
	}
	if (space->getLeft() != nullptr && space->getRight() != nullptr) {
		if (space->getLeft()->getSubspaceCount() > space->getRight()->getSubspaceCount()) {
			addClientRecursive(client, space->getRight().get());
		} else {
			addClientRecursive(client, space->getLeft().get());
		}
		return;
	}
	if (space->getSize().x > space->getSize().y) {
		splitSpace(client, space, true);
	} else {
		splitSpace(client, space, false);
	}
}
void TreeLayoutManager::placeClientInSpace(Client* client, Space* space) {
	client->move(space->getPos().x + border_size_ + gap_ / 2, space->getPos().y + border_size_ + gap_ / 2);
	client->resize(space->getSize().x - (border_size_ * 2)- gap_, space->getSize().y - (border_size_ * 2) - gap_);
	client->restack();
	space->setClient(client);
}
void TreeLayoutManager::splitSpace(Client* client, Space* space, bool splitAlongX) {
	Point sizeLeft, sizeRight;
	Point posRight;
	if (splitAlongX) {
		sizeLeft = Point(space->getSize().x / 2, space->getSize().y);
		sizeRight = Point(space->getSize().x - sizeLeft.x, space->getSize().y);
		posRight = Point(space->getPos().x + sizeLeft.x, space->getPos().y);
	} else {
		sizeLeft = Point(space->getSize().x, space->getSize().y / 2);
		sizeRight = Point(space->getSize().x, space->getSize().y - sizeLeft.y);
		posRight = Point(space->getPos().x, space->getPos().y + sizeLeft.y);
	}
	auto leftSpace = std::make_unique<Space>(space->getPos(), sizeLeft, space_count_++, space);
	auto rightSpace = std::make_unique<Space>(posRight, sizeRight, space_count_++, space);
	placeClientInSpace(space->getClient(), leftSpace.get());
	placeClientInSpace(client, rightSpace.get());
	space->setLeft(std::move(leftSpace));
	space->setRight(std::move(rightSpace));
	space->setClient(nullptr);
	space->incSubSpaceCount();
	while(space->getParent() != nullptr) {
		space->getParent()->incSubSpaceCount();
		space = space->getParent();
	}
}

void TreeLayoutManager::growSpaceX(Client *client) {
	Space* space = findSpace(client);
	if (space == rootSpace_) {
		return;
	}
	if (space->getParent()->getLeft().get() == space) {
		space->setSize(Point(space->getSize().x + 1, space->getSize().y));
		space->getClient()->resize(space->getSize().x + 1, space->getSize().y);
		if (space->getParent()->getRight()->getClient() != nullptr) {
			space->getParent()->getRight()->setSize(Point(space->getParent()->getRight()->getSize().x - 1, space->getParent()->getRight()->getSize().y));
			space->getParent()->getRight()->getClient()->resize(space->getParent()->getRight()->getSize().x - 1, space->getParent()->getRight()->getSize().y);
			space->getParent()->getRight()->getClient()->move(space->getParent()->getRight()->getPos().x + 1, space->getParent()->getRight()->getPos().y);
		} else {
			space->getParent()->getRight()->setSize(Point(space->getParent()->getRight()->getSize().x - 1, space->getParent()->getRight()->getSize().y));

		}
		space->getParent()->getRight()->setSize(Point(space->getParent()->getRight()->getSize().x - 1, space->getParent()->getRight()->getSize().y));
	}
}
