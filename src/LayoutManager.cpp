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
 * @file LayoutManager.cpp
 * @brief LayoutManager class implementation.
 * @date 2021-06-23
 *
 */
#include "LayoutManager.hpp"

LayoutManager::LayoutManager(Display *display, Window root) : display_(display), rootWindow_(root),space_count_(0){

}


void LayoutManager::updateGeometry() {

}

LayoutManager::Space * LayoutManager::findSpace(Client *client) {
	return nullptr;
}
LayoutManager::Space * LayoutManager::findSpace(int index) {
	return nullptr;
}

void LayoutManager::addClient(Client *client) {

}


LayoutManager::SpaceNotFoundException::SpaceNotFoundException(const std::string &message) : runtime_error(message) {

}

const char *LayoutManager::SpaceNotFoundException::what() const noexcept {
	return runtime_error::what();
}

LayoutManager::Space::Space(LayoutManager::Point pos, LayoutManager::Point size, int index,
							LayoutManager::Space *parent) :
		pos_(pos), size_(size), index_(index), subspace_count_(1),parent_(parent),left_(nullptr),right_(nullptr){

}

const LayoutManager::Point &LayoutManager::Space::getPos() const {
	return pos_;
}
void LayoutManager::Space::incSubSpaceCount() {
	subspace_count_ ++;
}
void LayoutManager::Space::setPos(const LayoutManager::Point &pos) {
	Space::pos_ = pos;
}

const LayoutManager::Point &LayoutManager::Space::getSize() const {
	return size_;
}

void LayoutManager::Space::setSize(const LayoutManager::Point &size) {
	Space::size_ = size;
}

int LayoutManager::Space::getIndex() const {
	return index_;
}

void LayoutManager::Space::setIndex(int index) {
	Space::index_ = index;
}

LayoutManager::Space *LayoutManager::Space::getParent() const {
	return parent_;
}

void LayoutManager::Space::setParent(LayoutManager::Space *parent) {
	Space::parent_ = parent;
}

const std::unique_ptr<LayoutManager::Space> &LayoutManager::Space::getRight() const {
	return right_;
}

void LayoutManager::Space::setRight(std::unique_ptr<Space> right) {
	this->right_ = std::move(right);
}

const std::unique_ptr<LayoutManager::Space> &LayoutManager::Space::getLeft() const {
	return left_;
}

void LayoutManager::Space::setLeft( std::unique_ptr<Space> left) {
	this->left_ = std::move(left);
}

Client *LayoutManager::Space::getClient() const {
	return client_;
}

void LayoutManager::Space::setClient(Client *client) {
	Space::client_ = client;
}

int LayoutManager::Space::getSubspaceCount() const {
	return subspace_count_;
}
