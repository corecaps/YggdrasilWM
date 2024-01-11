//
// Created by corecaps on 1/12/24.
//

#include "TreeLayoutManager.hpp"

TreeLayoutManager::TreeLayoutManager(Display *display, Window root) : LayoutManager(display, root) {

}

TreeLayoutManager::~TreeLayoutManager() {

}

void TreeLayoutManager::updateGeometry() {
	LayoutManager::updateGeometry();
}

LayoutManager::Space *TreeLayoutManager::findSpace(Client *client) {
	return LayoutManager::findSpace(client);
}

LayoutManager::Space *TreeLayoutManager::findSpace(int index) {
	return LayoutManager::findSpace(index);
}

void TreeLayoutManager::addClient(Client *client) {
	LayoutManager::addClient(client);
}
