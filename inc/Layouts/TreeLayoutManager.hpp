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
 * @date 2024-02-05
 */


#ifndef YGGDRASILWM_TREELAYOUTMANAGER_HPP
#define YGGDRASILWM_TREELAYOUTMANAGER_HPP
#include "LayoutManager.hpp"
#include <iostream>
#include "Layouts/BinarySpace.hpp"
/**
 * @class TreeLayoutManager
 * @brief TreeLayoutManager class
 * this class is a subclass of LayoutManager
 * it handles the tree style layout
 * When a client is added, it looks recursively for
 * the biggest space and splits it in two
 * placing the new client in the left space
 * and the old client in the right space
 */
class TreeLayoutManager :public LayoutManager {
public:
/**
 * @fn TreeLayoutManager(Display* display, Window root,int sizeX,int sizeY,int posX,int posY)
 * @brief Construct a new Tree Layout Manager object
 * the size and position of the root space should take
 * account of the bar size and position and the border size
 * @param display
 * @param root
 * @param sizeX
 * @param sizeY
 * @param posX
 * @param posY
 * @param borderSize
 * @param gap
 * @param barHeight
 */
	TreeLayoutManager(Display* display,
					  Window root,
					  int sizeX,
					  int sizeY,
					  int posX,
					  int posY,
					  int borderSize,
					  int gap,
					  int barHeight);
/**
 * @brief Destroy the Tree Layout Manager object
 */
	~TreeLayoutManager() override;
/**
 * @fn void TreeLayoutManager::updateGeometry()
 * @brief update the geometry of the layout (resize)
*/
	void	updateGeometry(unsigned int sizeX, unsigned int sizeY, unsigned int posX, unsigned int posY) override;
	void	reSize(const Point &size, const Point &pos) override;
	void	recursiveResize(const Point &size, const Point &pos, BinarySpace *space);
/**
 * @fn BinarySpace* TreeLayoutManager::findSpace(Client* client)
 * @brief find the space containing the client
 * it looks recursively for the space containing the client
 * @param client
 * @return
 */
	BinarySpace	*findSpace(Client* client);
/**
 * @fn BinarySpace* TreeLayoutManager::findSpaceRecursive(Client* client, BinarySpace* space)
 * @brief recursive method to find the space containing the client
 * @param client
 * @param space
 * @return
 */
	BinarySpace * findSpaceRecursive(Client *client, BinarySpace * space);
	BinarySpace	*findSpace(int index);
/**
 * @fn void TreeLayoutManager::addClient(Client* client)
 * @brief add a client to the layout
 * it looks for the biggest space and splits it in two
 * @param client
 * @see addClientRecursive
 * @see placeClientInSpace
 */
	void	addClient(std::shared_ptr<Client> client) override;
/**
 * @fn void TreeLayoutManager::removeClientRecursive(Client* client, BinarySpace* space)
 * @brief recursive method to add a client to the layout
 * @param client
 * @param space
 */
	void	addClientRecursive(std::shared_ptr<Client> client, BinarySpace *space);
/**
 * @fn void TreeLayoutManager::placeClientInSpace(Client* client, BinarySpace* space)
 * @brief place a client in a space
 * moves and resizes the client window to fit the space
 * restack the client window
 * @param client
 * @param space
 */
	void	placeClientInSpace(const std::shared_ptr<Client>& client, BinarySpace *space);
/**
 * @fn void TreeLayoutManager::splitSpace(Client* client, BinarySpace* space, bool splitAlongX)
 * @brief split a space in two
 * Move the client to the left space and the old client to the right space
 * increase the subspaces counter of all the parent spaces
 * @param client
 * @param space
 * @param splitAlongX
 */
	void	splitSpace(const std::shared_ptr<Client>& client, BinarySpace *space, bool splitAlongX);
/**
 * @fn void TreeLayoutManager::removeClient(Client* client)
 * @brief remove a client from the layout
 * use the recursive method to remove the client
 * @param client
 */
	void	removeClient(Client *client) override;
/**
 * @fn void TreeLayoutManager::removeClientRecursive(Client* client, BinarySpace* space)
 * @brief recursive method to remove a client from the layout
 * @param client
 * @param space
 */
	void	removeClientRecursive(Client *client, BinarySpace* space);
/**
 * @fn void TreeLayoutManager::growSpace(Client* client)
 * @brief grow the space of the client in the x axis
 * @todo this method is not implemented yet
 * @param client
 */
	void growSpace(Client *client, int inc);
	void recursiveShrinkSiblingSpace(BinarySpace *space, int inc, bool vertical);
/**
 * @fn void TreeLayoutManager::shrinkSpaceX(Client* client)
 * @brief shrink the space of the client in the y axis
 * @todo this method is not implemented yet
 * @param client
 */
	void	shrinkSpace(Client *client);
private:

	std::unique_ptr<BinarySpace>			rootSpace_;

	void deleteSpace(BinarySpace *space);
};
#endif //YGGDRASILWM_TREELAYOUTMANAGER_HPP
