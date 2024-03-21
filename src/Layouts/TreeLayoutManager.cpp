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
 * @date 2024-03-21
 */
#include "Layouts/TreeLayoutManager.hpp"

TreeLayoutManager::TreeLayoutManager(Display *display,
									 Window root,
									 int sizeX,
									 int sizeY,
									 int posX,
									 int posY,
									 int borderSize,
									 int gap,
									 int barHeight) :
	LayoutManager(display, root, sizeX, sizeY, posX, posY, gap, barHeight) {
	Point pos(posX, posY);
	Point size(sizeX - borderSize, sizeY - borderSize);
	this->rootSpace_ = std::make_unique<BinarySpace>(pos, size, 0);
}
void TreeLayoutManager::deleteSpace(BinarySpace *space) {
	if (space->getLeft() != nullptr) {
		deleteSpace(space->getLeft().get());
	}
	if (space->getRight() != nullptr) {
		deleteSpace(space->getRight().get());
	}
	delete space;
}
TreeLayoutManager::~TreeLayoutManager() {}
void TreeLayoutManager::updateGeometry(unsigned int sizeX, unsigned int sizeY, unsigned int posX, unsigned int posY) {
	reSize(Point(sizeX,sizeY),Point(posX,posY));
}
BinarySpace * TreeLayoutManager::findSpaceRecursive(Client *client,
													BinarySpace * space) {
	if (space->getClient().get() == client) {
		return space;
	}
	if (space->getLeft() != nullptr) {
		BinarySpace * leftSpace = findSpaceRecursive(client, space->getLeft().get());
		if (leftSpace != nullptr) {
			return leftSpace;
		}
	}
	if (space->getRight() != nullptr) {
		BinarySpace * rightSpace = findSpaceRecursive(client, space->getRight().get());
		if (rightSpace != nullptr) {
			return rightSpace;
		}
	}
	return nullptr;
}
BinarySpace * TreeLayoutManager::findSpace(Client *client) {
	BinarySpace* space = rootSpace_.get();
	return findSpaceRecursive(client, space);
}
BinarySpace * TreeLayoutManager::findSpace(int index) {
	return nullptr;
}
void TreeLayoutManager::removeClient(Client* client) {
	removeClientRecursive(client, rootSpace_.get());
}
void TreeLayoutManager::removeClientRecursive(Client* client, BinarySpace* space) {
	if (space->getClient().get() == client) {
		space->setClient(nullptr);
		if (space != rootSpace_.get()) {
			bool isLeftChild = (space->getParent()->getLeft().get() == space);
			BinarySpace* siblingSpace = isLeftChild ? space->getParent()->getRight().get() : space->getParent()->getLeft().get();
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
void TreeLayoutManager::addClient(std::shared_ptr<Client> client) {
	addClientRecursive(client, rootSpace_.get());
}
void TreeLayoutManager::addClientRecursive(std::shared_ptr<Client> client, BinarySpace* space) {
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
void TreeLayoutManager::placeClientInSpace(const std::shared_ptr<Client>& client, BinarySpace* space) {
	client->move(static_cast<int>(space->getPos().x) + border_size_ + gap_ / 2,static_cast<int>(space->getPos().y) + border_size_ + gap_ / 2);
	client->resize(space->getSize().x - (border_size_ * 2)- gap_, space->getSize().y - (border_size_ * 2) - gap_);
	client->restack();
	if (space->getClient().get() != client.get())
		space->setClient(client);
}
void TreeLayoutManager::splitSpace(const std::shared_ptr<Client>& client, BinarySpace* space, bool splitAlongX) {
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
	auto leftSpace = std::make_unique<BinarySpace>(space->getPos(), sizeLeft, space_count_++, space);
	auto rightSpace = std::make_unique<BinarySpace>(posRight, sizeRight, space_count_++, space);
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

void TreeLayoutManager::growSpace(Client *client, int inc) {
	BinarySpace *space = findSpace(client);
	if (space == nullptr || space == rootSpace_.get()) {
		return;
	}
	bool isLeftChild = (space->getParent()->getLeft().get() == space);
	int dx = std::abs((int)space->getSize().x - (int)space->getParent()->getSize().x);
	int dy = std::abs((int)space->getSize().y - (int)space->getParent()->getSize().y);
	bool vertical;
	if (std::max(dx, dy) == dx) {
		vertical = false;
	} else {
		vertical = true;
	}
	BinarySpace *siblingSpace = isLeftChild ? space->getParent()->getRight().get()
											: space->getParent()->getLeft().get();
	space->setSize(vertical ? Point(space->getSize().x, space->getSize().y + inc) : Point(space->getSize().x + inc,
																						  space->getSize().y));
	if (!isLeftChild) {
		space->setPos(vertical ? Point(space->getPos().x, space->getPos().y - inc) : Point(space->getPos().x - inc,
																						  space->getPos().y));
	} else {
		siblingSpace->setPos(vertical ? Point(siblingSpace->getPos().x, siblingSpace->getPos().y + inc) : Point(
				siblingSpace->getPos().x + inc, siblingSpace->getPos().y));
	}
	if (space->getClient() != nullptr) {
		placeClientInSpace(space->getClient(), space);
	}
	siblingSpace->setSize(vertical ? Point(siblingSpace->getSize().x, siblingSpace->getSize().y - inc) : Point(
			siblingSpace->getSize().x - inc, siblingSpace->getSize().y));
	if (siblingSpace->getClient() != nullptr) {
		placeClientInSpace(siblingSpace->getClient(), siblingSpace);
	} else {
		recursiveShrinkSiblingSpace(siblingSpace, inc, vertical);
	}
}
void TreeLayoutManager::recursiveShrinkSiblingSpace(BinarySpace *space, int inc, bool vertical) {
	if (space->getLeft() != nullptr) {
		recursiveShrinkSiblingSpace(space->getLeft().get(), inc, vertical);
	}
	if (space->getRight() != nullptr) {
		recursiveShrinkSiblingSpace(space->getRight().get(), inc, vertical);
	}
	space->setSize(vertical ? Point(space->getSize().x, space->getSize().y - inc) : Point(space->getSize().x - inc,
																						  space->getSize().y));
	if (space->getClient() != nullptr) {
		placeClientInSpace(space->getClient(), space);
	}
}
void TreeLayoutManager::reSize(const Point &size,
							   const Point &pos) {
	if (rootSpace_->getSize().x == size.x && rootSpace_->getSize().y == size.y) {
		return;
	}
	recursiveResize(size, pos,rootSpace_.get());
}

void TreeLayoutManager::recursiveResize(const Point &size,
										const Point &pos,
										BinarySpace * space) {
	Point oldSize = space->getSize();
	space->setSize(size);
	space->setPos(pos);
	if (space->getClient() != nullptr) {
		placeClientInSpace(space->getClient(), space);
	}
	if (space->getLeft() != nullptr) {
		Point leftPos = pos;
		Point leftSize = size;
		Point rightPos = pos;
		Point rightSize = size;
		if (space->getLeft()->getSize().x == oldSize.x) {
			leftSize.y = size.y / 2;
			rightSize.y = size.y - leftSize.y;
			rightPos.y = pos.y + leftSize.y;
		} else {
			leftSize.x = size.x / 2;
			rightSize.x = size.x - leftSize.x;
			rightPos.x = pos.x + leftSize.x;
		}
		recursiveResize(leftSize, leftPos, space->getLeft().get());
		recursiveResize(rightSize, rightPos, space->getRight().get());
	} 
}


