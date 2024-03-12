#include <iostream>
#include <sstream>
#include <iomanip>
#include "clock.hpp"

ClockWidget::ClockWidget() : display(nullptr),
							 parentWindow(0),
							 window(0),
							 x(0),
							 y(0),
							 width(0),
							 height(0) {
}

ClockWidget::~ClockWidget() {

}

Window ClockWidget::initialize(Display *d, Window pW, int x_, int y_, int width_, int height_) {
	display = d;
	parentWindow = pW;
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	window = XCreateSimpleWindow(display, parentWindow, x, y, width, height, 0, 0, 0xFFFFFF);
	XMapWindow(display, window);
	return window;
}

void ClockWidget::draw() {
	int screen = DefaultScreen(display);
	std::stringstream message;
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm localTime{};
	localtime_r(&now, &localTime);
	message <<"| "<< std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << " | ";
	XClearWindow(display, window);
	XDrawString(display, window, DefaultGC(display, screen), 10, height / 2, message.str().c_str(), message.str().size());
	XFlush(display);
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
