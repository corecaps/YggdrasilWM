#include "groupw.hpp"
#include <iostream>
#include <sstream>
GroupWidget::GroupWidget() : display(nullptr),
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
							 data(){}

GroupWidget::~GroupWidget() = default;

Window GroupWidget::initialize(Display *d,
							   Window pW,
							   int x_,
							   int y_,
							   int width_,
							   int height_,
							   std::string font_,
							   unsigned long bgColor_,
							   unsigned long fgColor_,
							   int fontSize) {
	display = d;
	parentWindow = pW;
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	bgColor = bgColor_;
	fgColor = fgColor_;
	int screen = DefaultScreen(display);
	window = XCreateSimpleWindow(display,
								 parentWindow,
								 x, y,
								 width, height,
								 0, 0, bgColor);
	XSetWindowBorder(display, window, 0x000000);
	XSetWindowBorderWidth(display, window, 1);
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
	renderColor.red = 0xff;
	renderColor.green = 0;
	renderColor.blue = 0;
	renderColor.alpha = 0xFFFF;
	XftColorAllocValue(display,
					   DefaultVisual(display, screen),
					   DefaultColormap(display, screen),
					   &renderColor,
					   &ftcolor);
	data["Groups"] = "";
	data["ActiveGroup"] = "";
	return window;
}

void GroupWidget::draw() {
	std::stringstream message;
	message << data["Groups"] << "[" << data["ActiveGroup"] << "]";
	std::cout << message.str() << std::endl;
	XftDrawString8(ftdraw,
				   &ftcolor,
				   fontStruct,
				   10,
				   height / 2,
				   (const FcChar8 *) message.str().c_str(),
				   message.str().size());
	XFlush(display);
}

void GroupWidget::handleEvent(XEvent &event) {

}

void GroupWidget::shutdown() {

}

void GroupWidget::setPosition(int x_, int y_) {

}

void GroupWidget::setSize(int width_, int height_) {

}

std::vector<std::string> GroupWidget::registerDataKey() {
	std::vector <std::string> keys;
	keys.emplace_back("Groups");
	keys.emplace_back("ActiveGroup");
	return keys;
}

void GroupWidget::unregisterDataKey(const std::string &key) {

}

void GroupWidget::updateData(const std::string &key, const std::string &value) {
	std::cout << "-----------\t Adding Data: " << key << " : " << value << std::endl;
	data[key] = value;
}
