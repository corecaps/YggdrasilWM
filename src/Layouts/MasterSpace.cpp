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
 * @file MasterSpace.cpp
 * @brief MasterSpace class implementation.
 * @date 2024-03-21
 */
#include "Layouts/MasterSpace.hpp"

MasterSpace::MasterSpace(Point pos,
						 Point size,
						 int index,
						 bool master,
						 bool vertical,
						 MasterSpace *parent) :
	pos_(pos),
	size_(size),
	index_(index),
	is_master_(master),
	is_vertical_(vertical),
	subspace_count_(0){
	if (parent) {
		parent_ = parent;
	}
}
MasterSpace::~MasterSpace() = default;
const Point &MasterSpace::getPos() const { return pos_; }
void MasterSpace::setPos(const Point &pos) { pos_ = pos; }
const Point &MasterSpace::getSize() const { return size_; }
void MasterSpace::setSize(const Point &size) { size_ = size; }
bool MasterSpace::isMaster() const { return is_master_; }
void MasterSpace::setIsMaster(bool isMaster) { is_master_ = isMaster; }
bool MasterSpace::isVertical() const { return is_vertical_; }
void MasterSpace::setIsVertical(bool isVertical) { is_vertical_ = isVertical; }
int MasterSpace::getIndex() const { return index_; }
void MasterSpace::setIndex(int index) { index_ = index; }
int MasterSpace::getSubspaceCount() const { return subspace_count_; }
void MasterSpace::setSubspaceCount(int subspaceCount) { subspace_count_ = subspaceCount; }
MasterSpace *MasterSpace::getParent() const { return parent_; }
void MasterSpace::setParent(MasterSpace *parent) { parent_ = parent; }
MasterSpace *MasterSpace::getMaster() const { return master_.get(); }
void MasterSpace::setMaster(std::unique_ptr<MasterSpace> master) { master_ = std::move(master); }
const std::vector<std::unique_ptr<MasterSpace>> &MasterSpace::getSlaves() const { return slaves_; }
const std::weak_ptr<Client> &MasterSpace::getClient() const { return client_; }
void MasterSpace::setClient(const std::weak_ptr<Client> &client) { client_ = client; }