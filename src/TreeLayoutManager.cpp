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
 * @date 2021-06-23
 */

#include <iostream>
#include "TreeLayoutManager.hpp"

TreeLayoutManager::TreeLayoutManager(Display *display, Window root) : LayoutManager(display, root) {
	Point size = Point(DisplayWidth(display, DefaultScreen(display)),
					   DisplayHeight(display, DefaultScreen(display))-30);
	this->rootSpace_ = new Space(Point(0, 30), size, 0);
}

TreeLayoutManager::~TreeLayoutManager() {
	delete rootSpace_;
}

void TreeLayoutManager::updateGeometry() {
	LayoutManager::updateGeometry();
}

LayoutManager::Space * TreeLayoutManager::findSpaceRecursive(Client *client, LayoutManager::Space * space) {
	if (space->getClient() == client) {
		return space;
	}
	// Recursively check left and right child spaces
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
	// Recursively search for the space containing the client
	return findSpaceRecursive(client, space);
}
LayoutManager::Space * TreeLayoutManager::findSpace(int index) {
	return LayoutManager::findSpace(index);
}

void TreeLayoutManager::removeClient(Client* client) {
	// Start the recursion from the root space
	removeClientRecursive(client, rootSpace_);
}
void TreeLayoutManager::removeClientRecursive(Client* client, Space* space) {
	if (space->getClient() == client) {
		space->setClient(nullptr);

		// Check if the space is not the root
		if (space != rootSpace_) {
			// Determine whether the current space is the left or right child of its parent
			bool isLeftChild = (space->getParent()->getLeft().get() == space);


			// Get the sibling space
			Space* siblingSpace = isLeftChild ? space->getParent()->getRight().get() : space->getParent()->getLeft().get();

			// Check if the sibling space has a client
			if (siblingSpace->getClient() != nullptr) {
				// Move the client from the sibling space to the parent space
				placeClientInSpace(siblingSpace->getClient(), space->getParent());

				// Set the sibling space's client to nullptr
				siblingSpace->setClient(nullptr);
			}
			// Remove the current space from its parent
			if (isLeftChild) {
				space->getParent()->setLeft(nullptr);
			} else {
				space->getParent()->setRight(nullptr);
			}
		}
		return;
	}

	// Recursively check left and right child spaces
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
	// If the space is empty, place the client in this space
	if (space->getClient() == nullptr && space->getLeft() == nullptr && space->getRight() == nullptr ) {
		placeClientInSpace(client, space);
		return;
	}
	// look for the space with the least subspaces
	if (space->getLeft() != nullptr && space->getRight() != nullptr) {
		if (space->getLeft()->getSubspaceCount() > space->getRight()->getSubspaceCount()) {
			addClientRecursive(client, space->getRight().get());
		} else {
			addClientRecursive(client, space->getLeft().get());
		}
		return;
	}
	// Split the space along the longest axis
	if (space->getSize().x > space->getSize().y) {
		splitSpace(client, space, true);
	} else {
		splitSpace(client, space, false);
	}
}

void TreeLayoutManager::placeClientInSpace(Client* client, Space* space) {
	client->move(space->getPos().x, space->getPos().y);
	client->resize(space->getSize().x, space->getSize().y);
	client->restack();
	space->setClient(client);
}

void TreeLayoutManager::splitSpace(Client* client, Space* space, bool splitAlongX) {
	// Create two child spaces
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
	// Create left and right child spaces
	auto leftSpace = std::make_unique<Space>(space->getPos(), sizeLeft, space_count_++, space);
	auto rightSpace = std::make_unique<Space>(posRight, sizeRight, space_count_++, space);
	// Move the current client to the left child space
	placeClientInSpace(space->getClient(), leftSpace.get());
	// Move the new client to the right child space
	placeClientInSpace(client, rightSpace.get());
	// Set left and right child spaces
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
	// Find the space containing the client
	Space* space = findSpace(client);
	// If the space is the root space, do nothing
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

