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
 * @date 2021-06-23
 * This class is responsible for managing the layout of the windows.
 */

#ifndef YGGDRASILWM_LAYOUTMANAGER_HPP
#define YGGDRASILWM_LAYOUTMANAGER_HPP

#include <memory>
#include <stdexcept>
#include "Client.hpp"


class LayoutManager {
public:
	struct Point {
		int	x;
		int	y;

		Point(int x, int y) : x(x), y(y) {}
		Point() : x(0), y(0) {}
	};

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
		Space(Point pos, Point size, int index, Space* parent = nullptr);
		const Point &getPos() const;
		void setPos(const Point &pos);
		const Point &getSize() const;
		void setSize(const Point &size);

		int getSubspaceCount() const;

		int getIndex() const;
		void setIndex(int index);
		Space *getParent() const;
		void setParent(Space *parent);
		const std::unique_ptr<Space> &getRight() const;
		void setRight(std::unique_ptr<Space> right);
		const std::unique_ptr<Space> &getLeft() const;
		void setLeft(std::unique_ptr<Space> left);
		Client *getClient() const;
		void setClient(Client *client);

		void incSubSpaceCount();
	};

	class SpaceNotFoundException : public std::runtime_error {
	public:
		explicit SpaceNotFoundException(const std::string& message);
		const char * what() const noexcept override;
	};

	LayoutManager(Display* display, Window root);
	virtual ~LayoutManager() = default;

	virtual void	updateGeometry();
	virtual Space	*findSpace(Client *client);
	virtual Space	*findSpace(int index);
	virtual void	addClient(Client* client);

protected:
	Space*	rootSpace_;
	int		screen_width_;
	int		screen_height_;
	int		space_count_;
	Display	*display_;
	Window	rootWindow_;
};

#endif //YGGDRASILWM_LAYOUTMANAGER_HPP
