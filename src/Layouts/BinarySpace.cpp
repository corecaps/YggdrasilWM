#include "Layouts/BinarySpace.hpp"
#include "Layouts/Point.hpp"

BinarySpace::BinarySpace(Point pos,
						 Point size,
						 int index,
						 BinarySpace *parent) :
		pos_(pos),
		size_(size),
		index_(index),
		subspace_count_(1),
		parent_(parent),
		left_(nullptr),
		right_(nullptr){}
const Point &BinarySpace::getPos() const { return pos_; }
void BinarySpace::incSubSpaceCount() { subspace_count_ ++; }
void BinarySpace::setPos(const Point &pos) { BinarySpace::pos_ = pos; }
const Point &BinarySpace::getSize() const { return size_; }
void BinarySpace::setSize(const Point &size) { BinarySpace::size_ = size; }
int BinarySpace::getIndex() const { return index_; }
void BinarySpace::setIndex(int index) { BinarySpace::index_ = index; }
BinarySpace *BinarySpace::getParent() const { return parent_; }
void BinarySpace::setParent(BinarySpace *parent) { BinarySpace::parent_ = parent;}
const std::unique_ptr<BinarySpace> &BinarySpace::getRight() const { return right_; }
void BinarySpace::setRight(std::unique_ptr<BinarySpace> right) { this->right_ = std::move(right); }
const std::unique_ptr<BinarySpace> &BinarySpace::getLeft() const { return left_; }
void BinarySpace::setLeft(std::unique_ptr<BinarySpace> left) { this->left_ = std::move(left); }
std::shared_ptr<Client>BinarySpace::getClient() const {
	auto c = client_.lock();
	if (c)
		return c;
	else
		return nullptr;
}
void BinarySpace::setClient(std::shared_ptr<Client> client) { BinarySpace::client_ = std::weak_ptr<Client>(client); }
int BinarySpace::getSubspaceCount() const {return subspace_count_; }