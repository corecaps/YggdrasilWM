#include <iostream>
#include "clock.hpp"

ClockWidget::ClockWidget() : display(nullptr),
							 parentWindow(0),
							 window(0),
							 x(0),
							 y(0),
							 width(0),
							 height(0) {
	std::cout << "ClockWidget created" << std::endl;
}

ClockWidget::~ClockWidget() {

}

void ClockWidget::initialize(Display *d, Window pW, int x_, int y_, int width_, int height_) {
	display = d;
	parentWindow = pW;
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	std::cout << "ClockWidget initialized" << std::endl;
}

void ClockWidget::draw() {
	std::cout << "ClockWidget draw" << std::endl;
}

void ClockWidget::handleEvent(XEvent &event) {

}

void ClockWidget::shutdown() {

}

void ClockWidget::setPosition(int x_, int y_) {

}

void ClockWidget::setSize(int width_, int height_) {

}

void ClockWidget::registerDataKey(const std::string &key) {

}

void ClockWidget::unregisterDataKey(const std::string &key) {

}

void ClockWidget::updateData(const std::string &key, const std::string &value) {

}
