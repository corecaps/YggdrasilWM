#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "clock.hpp"


ClockWidget::ClockWidget() : display(nullptr),
							 parentWindow(0),
							 window(0),
							 x(0),
							 y(0),
							 width(0),
							 height(0),
							 bgColor(0),
							 fgColor(0),
							 ftdraw(nullptr),
							 fontStruct(nullptr),
							 ftcolor(),
							 timeFormat("%Y-%m-%d %H:%M:%S"),
							 fontName(""),
							 color(""),
							 backgroundColor(""),
							 borderColor(""),
							 borderWidth(""),
							 padding(""),
							 margin(""),
							 position(""),
							 top(""),
							 bottom(""),
							 left(""),
							 right(""),
							 dataKey(""),
							 dataValue(""),
							 lastUpdate(std::chrono::system_clock::now()) {
}

ClockWidget::~ClockWidget() {

}

Window ClockWidget::initialize(Display *d, Window pW, int x_, int y_, int width_, int height_, std::string font_,
							   unsigned long bgColor_, unsigned long fgColor_, int fontSize) {
	display = d;
	parentWindow = pW;
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	bgColor = bgColor_;
	fgColor = fgColor_;
	int screen = DefaultScreen(display);
	window = XCreateSimpleWindow(display, parentWindow, x, y, width, height, 0, 0, bgColor);
	XMapWindow(display, window);
	ftdraw = XftDrawCreate(display,
						   window,
						   DefaultVisual(display, screen),
						   DefaultColormap(display, screen));
	if (!ftdraw) {
		std::cerr << "XftDrawCreate failed" << std::endl;
	}
	fontName = font_ + ":size=" + std::to_string(fontSize);
	fontStruct = XftFontOpenName(display, screen, fontName.c_str());
	if (!fontStruct) {
		std::cerr << "XftFontOpenName failed" << std::endl;
	}
	XRenderColor renderColor;
	renderColor.red = 0;
	renderColor.green = 0;
	renderColor.blue = 0;
	renderColor.alpha = 0xFFFF;
	XftColorAllocValue(display,
					   DefaultVisual(display, screen),
					   DefaultColormap(display, screen),
					   &renderColor,
					   &ftcolor);
	if (!ftcolor.pixel) {
		std::cerr << "XftColorAllocValue failed" << std::endl;
	}
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
	XftDrawStringUtf8(ftdraw,
					  &ftcolor,
					  fontStruct,
					  10, height / 2,
					  (const FcChar8*)message.str().c_str(),
					  message.str().size());
//	XDrawString(display, window, DefaultGC(display, screen), 10, height / 2, message.str().c_str(), message.str().size());
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

std::vector<std::string> ClockWidget::registerDataKey() {
	std::vector <std::string> keys;
	keys.push_back("");
	return keys;
}

void ClockWidget::unregisterDataKey(const std::string &key) {
}

void ClockWidget::updateData(const std::string &key, const std::string &value) {

}
