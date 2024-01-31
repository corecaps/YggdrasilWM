/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
 * @file TreeLayoutManager
 * @brief handle Tree Style Layout.
 * @date 2021-06-23
 */

#include <iostream>
#include "TreeLayoutManager.hpp"

TreeLayoutManager::TreeLayoutManager(Display *display, Window root) : LayoutManager(display, root) {
	Point size = Point(DisplayWidth(display, DefaultScreen(display)),
					   DisplayHeight(display, DefaultScreen(display)));
	this->rootSpace_ = new Space(Point(0, 0), size, 0);
}

TreeLayoutManager::~TreeLayoutManager() {

}

void TreeLayoutManager::updateGeometry() {
	LayoutManager::updateGeometry();
}

LayoutManager::Space * TreeLayoutManager::findSpace(Client *client) {
	return LayoutManager::findSpace(client);
}

LayoutManager::Space * TreeLayoutManager::findSpace(int index) {
	return LayoutManager::findSpace(index);
}
void TreeLayoutManager::addClient(Client* client) {
	// Start the recursion from the root space
	addClientRecursive(client, rootSpace_);
}
void TreeLayoutManager::RemoveClient(Client* client) {
	// Start the recursion from the root space
	RemoveClientRecursive(client, rootSpace_);
}
void TreeLayoutManager::RemoveClientRecursive(Client *client, Space* space) {
	if (space->getClient() == client) {
		space->setClient(nullptr);
		return;
	}
	if (space->getLeft() != nullptr) {
		RemoveClientRecursive(client, space->getLeft().get());
	}
	if (space->getRight() != nullptr) {
		RemoveClientRecursive(client, space->getRight().get());
	}
}

void TreeLayoutManager::addClientRecursive(Client* client, Space* space) {
	// If the space is empty, place the client in this space
	if (space->getClient() == nullptr && space->getLeft() == nullptr && space->getRight() == nullptr ) {
		placeClientInSpace(client, space);
		return;
	}

	// If the space is not empty, split the biggest dimension
	if (space->getSize().x > space->getSize().y) {
		// Split along the x-axis
		if (space->getLeft() != nullptr) {
			addClientRecursive(client, space->getLeft().get());
			return;
		}
		splitSpace(client, space, true);
	} else {
		if (space->getRight() != nullptr) {
			addClientRecursive(client, space->getRight().get());
			return;
		}
		// Split along the y-axis
		splitSpace(client, space, false);
	}
}

void TreeLayoutManager::placeClientInSpace(Client* client, Space* space) {
	// Set the client to occupy the entire space
	client->move(space->getPos().x, space->getPos().y);
	client->resize(space->getSize().x, space->getSize().y);

	// Set the client for the space
	space->setClient(client);
}

void TreeLayoutManager::splitSpace(Client* client, Space* space, bool splitAlongX) {
	// Create two child spaces
	Point sizeLeft, sizeRight;
	if (splitAlongX) {
		sizeLeft = Point(space->getSize().x / 2, space->getSize().y);
		sizeRight = Point(space->getSize().x - sizeLeft.x, space->getSize().y);
	} else {
		sizeLeft = Point(space->getSize().x, space->getSize().y / 2);
		sizeRight = Point(space->getSize().x, space->getSize().y - sizeLeft.y);
	}

	// Create left and right child spaces
	auto leftSpace = std::make_unique<Space>(space->getPos(), sizeLeft, space_count_++, space);
	auto rightSpace = std::make_unique<Space>(Point(space->getPos().x + sizeLeft.x, space->getPos().y),
											  sizeRight, space_count_++, space);

	// Move the current client to the left child space
	placeClientInSpace(space->getClient(), leftSpace.get());

	// Move the new client to the right child space
	placeClientInSpace(client, rightSpace.get());

	// Set left and right child spaces
	space->setLeft(std::move(leftSpace));
	space->setRight(std::move(rightSpace));
	space->setClient(nullptr);
}