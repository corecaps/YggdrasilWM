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
 * @file MasterSpace.hpp
 * @brief MasterSpace class header.
 * @date 2024-03-21
 */

#ifndef YGGDRASILWM_MASTERSPACE_HPP
#define YGGDRASILWM_MASTERSPACE_HPP
#include "Layouts/Point.hpp"
#include <memory>
#include <vector>
class Client;
class MasterSpace {
public:
	MasterSpace(Point pos,
				Point size,
				int index,
				bool master,
				bool vertical,
				MasterSpace* parent = nullptr);
	~MasterSpace();
	const Point &getPos() const;
	void setPos(const Point &pos);
	const Point &getSize() const;
	void setSize(const Point &size);
	bool isMaster() const;
	void setIsMaster(bool isMaster);
	bool isVertical() const;
	void setIsVertical(bool isVertical);
	int getIndex() const;
	void setIndex(int index);
	int getSubspaceCount() const;
	void setSubspaceCount(int subspaceCount);
	MasterSpace *getParent() const;
	void setParent(MasterSpace *parent);
	MasterSpace *getMaster() const;
	void setMaster(std::unique_ptr<MasterSpace>master);
	const std::vector<std::unique_ptr<MasterSpace>> &getSlaves() const;
	const std::weak_ptr<Client> &getClient() const;

	void setClient(const std::weak_ptr<Client> &client);

private:
	Point										pos_;
	Point										size_;
	bool										is_master_;
	bool										is_vertical_;
	int											index_;
	int											subspace_count_;
	MasterSpace*								parent_;
	std::unique_ptr<MasterSpace>				master_;
	std::vector<std::unique_ptr<MasterSpace>>	slaves_;
	std::weak_ptr<Client>						client_;
};
#endif //YGGDRASILWM_MASTERSPACE_HPP