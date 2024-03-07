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
 * @date 2024-02-07
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
	LayoutManager(display, root, sizeX, sizeY, posX, posY, gap, borderSize) {
	Point pos(posX, posY);
	Point size(sizeX - borderSize, sizeY - borderSize);
	this->rootSpace_ = std::make_unique<BinarySpace>(pos, size, 0);
}
void deleteSpace(TreeLayoutManager::BinarySpace *space) {
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
TreeLayoutManager::BinarySpace * TreeLayoutManager::findSpaceRecursive(Client *client,
																	   TreeLayoutManager::BinarySpace * space) {
	if (space->getClient().get() == client) {
		return space;
	}
	if (space->getLeft() != nullptr) {
		TreeLayoutManager::BinarySpace * leftSpace = findSpaceRecursive(client, space->getLeft().get());
		if (leftSpace != nullptr) {
			return leftSpace;
		}
	}
	if (space->getRight() != nullptr) {
		TreeLayoutManager::BinarySpace * rightSpace = findSpaceRecursive(client, space->getRight().get());
		if (rightSpace != nullptr) {
			return rightSpace;
		}
	}
	return nullptr;
}
TreeLayoutManager::BinarySpace * TreeLayoutManager::findSpace(Client *client) {
	BinarySpace* space = rootSpace_.get();
	return findSpaceRecursive(client, space);
}
TreeLayoutManager::BinarySpace * TreeLayoutManager::findSpace(int index) {
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

void TreeLayoutManager::growSpaceX(Client *client) {
	BinarySpace* space = findSpace(client);
	if (space == rootSpace_.get()) {
		return;
	}
	if (space->getParent()->getLeft().get() == space) {
		space->setSize(Point(space->getSize().x + 2, space->getSize().y));
		if (space->getClient() != nullptr) {
			placeClientInSpace(space->getClient(), space);
		}
		else {
			BinarySpace *leftChild = space->getLeft().get();
			BinarySpace *rightChild = space->getRight().get();
			while (leftChild != nullptr) {
				leftChild->setSize(Point(leftChild->getSize().x + 1, leftChild->getSize().y));
				leftChild = leftChild->getLeft().get();
			}
			while (rightChild != nullptr) {
				rightChild->setSize(Point(rightChild->getSize().x + 1, rightChild->getSize().y));
				rightChild = rightChild->getRight().get();
			}
		}
		if (space->getParent()->getRight() != nullptr) {
			space->getParent()->getRight()->setSize(
						Point(space->getParent()->getRight()->getSize().x - 2,
							  space->getParent()->getRight()->getSize().y));
			if (space->getParent()->getRight()->getClient() != nullptr) {
				placeClientInSpace(space->getParent()->getRight()->getClient(), space->getParent()->getRight().get());
			} else {
				BinarySpace *leftChild = space->getParent()->getRight()->getLeft().get();
				BinarySpace *rightChild = space->getParent()->getRight()->getRight().get();
				while (leftChild != nullptr) {
					leftChild->setSize(Point(leftChild->getSize().x - 1, leftChild->getSize().y));
					leftChild = leftChild->getLeft().get();
				}
				while (rightChild != nullptr) {
					rightChild->setSize(Point(rightChild->getSize().x - 1, rightChild->getSize().y));
					rightChild = rightChild->getRight().get();
				}
			}
		}
	}
}

void TreeLayoutManager::reSize(const LayoutManager::Point &size,
							   const LayoutManager::Point &pos) {
	if (rootSpace_->getSize().x == size.x && rootSpace_->getSize().y == size.y) {
		return;
	}
	recursiveResize(size, pos,rootSpace_.get());
}

void TreeLayoutManager::recursiveResize(const LayoutManager::Point &size,
										const LayoutManager::Point &pos,
										TreeLayoutManager::BinarySpace * space) {
	LayoutManager::Point oldSize = space->getSize();
	LayoutManager::Point oldPos = space->getPos();
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

TreeLayoutManager::BinarySpace::BinarySpace(LayoutManager::Point pos,
											LayoutManager::Point size,
											int index,
											TreeLayoutManager::BinarySpace *parent) :
		pos_(pos),
		size_(size),
		index_(index),
		subspace_count_(1),
		parent_(parent),
		left_(nullptr),
		right_(nullptr){}

const TreeLayoutManager::Point &TreeLayoutManager::BinarySpace::getPos() const { return pos_; }
void TreeLayoutManager::BinarySpace::incSubSpaceCount() { subspace_count_ ++; }
void TreeLayoutManager::BinarySpace::setPos(const LayoutManager::Point &pos) { BinarySpace::pos_ = pos; }
const TreeLayoutManager::Point &TreeLayoutManager::BinarySpace::getSize() const { return size_; }
void TreeLayoutManager::BinarySpace::setSize(const LayoutManager::Point &size) { BinarySpace::size_ = size; }
int TreeLayoutManager::BinarySpace::getIndex() const { return index_; }
void TreeLayoutManager::BinarySpace::setIndex(int index) { BinarySpace::index_ = index; }
TreeLayoutManager::BinarySpace *TreeLayoutManager::BinarySpace::getParent() const { return parent_; }
void TreeLayoutManager::BinarySpace::setParent(TreeLayoutManager::BinarySpace *parent) { BinarySpace::parent_ = parent;}
const std::unique_ptr<TreeLayoutManager::BinarySpace> &TreeLayoutManager::BinarySpace::getRight() const { return right_; }
void TreeLayoutManager::BinarySpace::setRight(std::unique_ptr<BinarySpace> right) { this->right_ = std::move(right); }
const std::unique_ptr<TreeLayoutManager::BinarySpace> &TreeLayoutManager::BinarySpace::getLeft() const { return left_; }
void TreeLayoutManager::BinarySpace::setLeft(std::unique_ptr<BinarySpace> left) { this->left_ = std::move(left); }
std::shared_ptr<Client>TreeLayoutManager::BinarySpace::getClient() const {
	auto c = client_.lock();
	if (c)
		return c;
	else
		return nullptr;
}
void TreeLayoutManager::BinarySpace::setClient(std::shared_ptr<Client> client) { BinarySpace::client_ = std::weak_ptr<Client>(client); }
int TreeLayoutManager::BinarySpace::getSubspaceCount() const {return subspace_count_; }