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
 * @file BinarySpace.hpp
 * @brief BinarySpace class header.
 * @date 2024-02-05
 */

#include <memory>
#include <stdexcept>
#include "Layouts/Point.hpp"
class Client;
/**
 * @class BinarySpace
 * @brief BinarySpace class
 * This class represents a space in the layout.
 * it's designed like a binary tree, with a parent, a right and a left child.
 * Only leaf spaces have clients.
 * @todo create an interface for Spaces and implement it in BinarySpace
 * @todo redefine BinarySpace not as a nested class but as a separate class
 */
class BinarySpace {
private:
	Point					pos_;
	Point					size_;
	int						index_;
	int						subspace_count_;
	BinarySpace*					parent_;
	std::unique_ptr<BinarySpace>	right_;
	std::unique_ptr<BinarySpace>	left_;
	std::weak_ptr<Client>	client_;
public:
/**
 * @fn BinarySpace(Point pos, Point size, int index, BinarySpace* parent = nullptr)
 * @brief Construct a new BinarySpace object
 * @param pos
 * @param size
 * @param index
 * @param parent
 */
	BinarySpace(Point pos, Point size, int index, BinarySpace* parent = nullptr);
	~BinarySpace() = default;
/**
 * @fn const Point LayoutManager::BinarySpace::getPos()
 * @brief Get the position of the space
 */
	const Point &getPos() const;
/**
 * @fn void LayoutManager::BinarySpace::setPos(const Point &pos)
 * @brief Set the position of the space
 * @param pos
 */
	void setPos(const Point &pos);
/**
 * @fn const Point LayoutManager::BinarySpace::getSize()
 * @brief Get the size of the space
 * @return
 */
	const Point &getSize() const;
/**
 * @fn void LayoutManager::BinarySpace::setSize(const Point &size)
 * @brief Set the size of the space
 * @param size
 */

	void setSize(const Point &size);
/**
 * @fn int LayoutManager::BinarySpace::getSubspaceCount()
 * @brief Get the number of subspaces
 */
	int getSubspaceCount() const;
/**
 * @fn int LayoutManager::BinarySpace::getIndex()
 * @brief Get the index of the space
 * @return
 */
	int getIndex() const;
/**
 * @fn void LayoutManager::BinarySpace::setIndex(int index)
 * @brief Set the index of the space
 * @param index
 */
	void setIndex(int index);
/**
 * @fn BinarySpace * LayoutManager::BinarySpace::getParent()
 * @brief Get the parent of the space
 * @return
 */
	BinarySpace *getParent() const;
/**
 * @fn void LayoutManager::BinarySpace::setParent(BinarySpace *parent)
 * @brief Set the parent of the space
 * @param parent
 */
	void setParent(BinarySpace *parent);
/**
 * @fn const std::unique_ptr<BinarySpace> & LayoutManager::BinarySpace::getRight()
 * @brief Get the right child of the space
 * @return
 */
	const std::unique_ptr<BinarySpace> &getRight() const;
/**
 * @fn void LayoutManager::BinarySpace::setRight(std::unique_ptr<BinarySpace> right)
 * @brief Set the right child of the space
 * @param right
 */
	void setRight(std::unique_ptr<BinarySpace> right);
/**
 * @fn const std::unique_ptr<BinarySpace> & LayoutManager::BinarySpace::getLeft()
 * @brief Get the left child of the space
 * @return
 */
	const std::unique_ptr<BinarySpace> &getLeft() const;
/**
 * @fn void LayoutManager::BinarySpace::setLeft(std::unique_ptr<BinarySpace> left)
 * @brief Set the left child of the space
 * @param left
 */
	void setLeft(std::unique_ptr<BinarySpace> left);
/**
 * @fn Client * LayoutManager::BinarySpace::getClient()
 * @brief Get the client of the space
 * @return
 */
	std::shared_ptr<Client> getClient() const;
/**
 * @fn void LayoutManager::BinarySpace::setClient(Client *client)
 * @brief Set the client of the space
 * @param client
 */
	void setClient(std::shared_ptr<Client> client);
/**
 * @fn void LayoutManager::BinarySpace::incSubSpaceCount()
 * @brief Increment the number of subspaces
 */
	void incSubSpaceCount();
};