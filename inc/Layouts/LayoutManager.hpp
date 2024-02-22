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
 * @file LayoutManager.hpp
 * @brief LayoutManager class header.
 * @date 2024-02-06
 * This class is responsible for managing the layout of the windows.
 */

#ifndef YGGDRASILWM_LAYOUTMANAGER_HPP
#define YGGDRASILWM_LAYOUTMANAGER_HPP

#include <memory>
#include <stdexcept>
#include "Client.hpp"
class BaseX11Wrapper;
/**
 * @class LayoutManager
 * @brief LayoutManager class
 * This a base class for the different layout managers.
 * You should not use this class directly, but use one of the derived classes.
 * @todo A factory should be used to create the layout managers.
 */
class LayoutManager {
public:
/**
 * @struct Point
 * @brief Point struct
 * This struct represents a 2D point
 */
	struct Point {
		unsigned int	x;
		unsigned int	y;

		Point(unsigned int x, unsigned int y) : x(x), y(y) {}
		Point() : x(0), y(0) {}
	};
/**
 * @class Space
 * @brief Space class
 * This class represents a space in the layout.
 * it's designed like a binary tree, with a parent, a right and a left child.
 * Only leaf spaces have clients.
 */
	class Space {
	private:
		Point					pos_;
		Point					size_;
		int						index_;
		int						subspace_count_;
		Space*					parent_;
		std::unique_ptr<Space>	right_;
		std::unique_ptr<Space>	left_;
		Client*					client_{};
	public:
/**
 * @fn Space(Point pos, Point size, int index, Space* parent = nullptr)
 * @brief Construct a new Space object
 * @param pos
 * @param size
 * @param index
 * @param parent
 */
		Space(Point pos, Point size, int index, Space* parent = nullptr);
/**
 * @fn const Point LayoutManager::Space::getPos()
 * @brief Get the position of the space
 */
		const Point &getPos() const;
/**
 * @fn void LayoutManager::Space::setPos(const Point &pos)
 * @brief Set the position of the space
 * @param pos
 */
		void setPos(const Point &pos);
/**
 * @fn const Point LayoutManager::Space::getSize()
 * @brief Get the size of the space
 * @return
 */
		const Point &getSize() const;
/**
 * @fn void LayoutManager::Space::setSize(const Point &size)
 * @brief Set the size of the space
 * @param size
 */

		void setSize(const Point &size);
/**
 * @fn int LayoutManager::Space::getSubspaceCount()
 * @brief Get the number of subspaces
 */
		int getSubspaceCount() const;
/**
 * @fn int LayoutManager::Space::getIndex()
 * @brief Get the index of the space
 * @return
 */
		int getIndex() const;
/**
 * @fn void LayoutManager::Space::setIndex(int index)
 * @brief Set the index of the space
 * @param index
 */
		void setIndex(int index);
/**
 * @fn Space * LayoutManager::Space::getParent()
 * @brief Get the parent of the space
 * @return
 */
		Space *getParent() const;
/**
 * @fn void LayoutManager::Space::setParent(Space *parent)
 * @brief Set the parent of the space
 * @param parent
 */
		void setParent(Space *parent);
/**
 * @fn const std::unique_ptr<Space> & LayoutManager::Space::getRight()
 * @brief Get the right child of the space
 * @return
 */
		const std::unique_ptr<Space> &getRight() const;
/**
 * @fn void LayoutManager::Space::setRight(std::unique_ptr<Space> right)
 * @brief Set the right child of the space
 * @param right
 */
		void setRight(std::unique_ptr<Space> right);
/**
 * @fn const std::unique_ptr<Space> & LayoutManager::Space::getLeft()
 * @brief Get the left child of the space
 * @return
 */
		const std::unique_ptr<Space> &getLeft() const;
/**
 * @fn void LayoutManager::Space::setLeft(std::unique_ptr<Space> left)
 * @brief Set the left child of the space
 * @param left
 */
		void setLeft(std::unique_ptr<Space> left);
/**
 * @fn Client * LayoutManager::Space::getClient()
 * @brief Get the client of the space
 * @return
 */
		Client *getClient() const;
/**
 * @fn void LayoutManager::Space::setClient(Client *client)
 * @brief Set the client of the space
 * @param client
 */
		void setClient(Client *client);
/**
 * @fn void LayoutManager::Space::incSubSpaceCount()
 * @brief Increment the number of subspaces
 */
		void incSubSpaceCount();
	};
/**
 * @brief SpaceNotFoundException class
 * This exception is thrown when a space is not found.
 */
	class SpaceNotFoundException : public std::runtime_error {
	public:
		explicit SpaceNotFoundException(const std::string& message);
		const char * what() const noexcept override;
	};
/**
 * @fn LayoutManager(Display* display, Window root)
 * @brief Construct a new LayoutManager object
 * @param display
 * @param root
 * @param size_x
 * @param size_y
 * @param pos_x
 * @param pos_y
 * @param gap
 * @param border_size
 * @param bar_height
 */
	LayoutManager(Display* display,
				  Window root,
				  int size_x,
				  int size_y,
				  int pos_x,
				  int pos_y,
				  int gap,
				  int border_size);
	virtual ~LayoutManager() = default;
/**
 * @fn virtual void LayoutManager::updateGeometry()
 * @brief updateGeometry this method is a residual from the old implementation.
 * it should be removed in the future.
 */
	virtual void	updateGeometry(unsigned int sizeX, unsigned int sizeY, unsigned int posX, unsigned int posY) = 0;
	virtual void reSize(const Point &size, const Point &pos) = 0;
	virtual void recursiveResize(const Point &size, const Point &pos, Space *space) = 0;
/**
 * @fn virtual Space * LayoutManager::findSpace(Client *client)
 * @brief find the space that contains the client
 * @param client
 * @return
 */
	virtual Space	*findSpace(Client *client) = 0;
/**
 * @fn virtual Space * LayoutManager::findSpace(int index)
 * @brief find the space with the given index
 * @param index
 * @return
 */
	virtual Space	*findSpace(int index) = 0;
/**
 * @fn virtual void LayoutManager::addClient(Client* client)
 * @brief add a client to the layout
 * @param client
 */
	virtual void	addClient(Client* client) = 0;
/**
 * @fn virtual void LayoutManager::removeClient(Client* client)
 * @brief remove a client from the layout
 * @param client
 */
	virtual void removeClient(Client* client) = 0;
protected:
	Space*							rootSpace_;
	int								screen_width_;
	int								screen_height_;
	int								gap_;
	int								border_size_;
	int								space_count_;
	Display							*display_;
	Window							rootWindow_;
};
#endif //YGGDRASILWM_LAYOUTMANAGER_HPP
