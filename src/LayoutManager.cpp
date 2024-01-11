/**
 * @file LayoutManager.cpp
 * @brief LayoutManager class implementation.
 * @date 2021-06-23
 *
 */
#include "LayoutManager.hpp"

LayoutManager::LayoutManager(Display *display, Window root) : display_(display), rootWindow_(root){

}

LayoutManager::~LayoutManager() {

}

void LayoutManager::updateGeometry() {

}

LayoutManager::Space *LayoutManager::findSpace(Client *client) {
	return nullptr;
}
LayoutManager::Space *LayoutManager::findSpace(int index) {
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
		pos_(pos), size_(size), index_(index), parent_(parent){

}

const LayoutManager::Point &LayoutManager::Space::getPos() const {
	return pos_;
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

void LayoutManager::Space::setRight(const std::unique_ptr<Space> &right) {
	Space::right_ = right;
}

const std::unique_ptr<LayoutManager::Space> &LayoutManager::Space::getLeft() const {
	return left_;
}

void LayoutManager::Space::setLeft(const std::unique_ptr<Space> &left) {
	Space::left_ = left;
}

Client *LayoutManager::Space::getClient() const {
	return client_;
}

void LayoutManager::Space::setClient(Client *client) {
	Space::client_ = client;
}
