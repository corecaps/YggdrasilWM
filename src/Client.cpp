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
 * @file Client.cpp
 * @brief Client class implementation.
 * @date 2024-02-11
 */

#include <cstring>
#include <utility>
#include "Client.hpp"
#include "Layouts/LayoutManager.hpp"
#include "Group.hpp"
#include "Logger.hpp"
#include "Config/ConfigHandler.hpp"
#include "Config/ConfigDataBindings.hpp"
#include "X11wrapper/baseX11Wrapper.hpp"
#include "YggdrasilExceptions.hpp"

Client::Client(Display *display,
			   Window root,
			   Window window,
			   std::shared_ptr<Group> g,
			   unsigned long inActiveColor,
			   int borderSize,
			   std::shared_ptr<BaseX11Wrapper> x11Wrapper)
		: display_(display),
		  root_(root),
		  group_(std::weak_ptr<Group>(g)),
		  window_(window),
		  frame_(0),
		  border_width(borderSize),
		  border_color(inActiveColor),
		  framed(false),
		  mapped(false),
		  wrapper(x11Wrapper)
{
	Atom wmClassAtom = wrapper->internAtom(display, "WM_CLASS", False);
	Atom actualType = 0;
	int actualFormat = 0;
	unsigned long nItems = 0, bytesAfter = 0;
	unsigned char* propData = 0;
	wrapper->getWindowProperty(display,
								   window,
								   wmClassAtom,
								   0,
								   1024,
								   False,
								   AnyPropertyType,
								   &actualType,
								   &actualFormat,
								   &nItems,
								   &bytesAfter,
								   &propData);
	if (actualType == XA_STRING && actualFormat == 8 && nItems > 1) {
		char* instanceName = reinterpret_cast<char*>(propData);
		char* className = instanceName + strlen(instanceName) + 1;
		this->class_ = className;
		this->title_ = instanceName;
	} else {
		Logger::GetInstance()->Log("Failed to get WM_CLASS property", L_ERROR);
		this->class_ = "Unknown";
		this->title_ = "Unknown";
	}
	wrapper->freeX(propData);
}
Client::~Client() {
	try {
		if (this->framed) {
			wrapper->destroyWindow(display_, frame_);
		}
	} catch (const X11Exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
	Logger::GetInstance()->Log("Client destroyed :" + title_, L_INFO);
}
void Client::frame() {
	const unsigned long BG_COLOR = 0x000000;
	auto g = group_.lock();
	if (g) {
		border_width = g->getBorderSize();
		border_color = g->getInactiveColor();
	} else {
		throw YggdrasilException("Group no longer exists");
	}
	if (this->framed)
		throw YggdrasilException("Client is already framed");
	XWindowAttributes x_window_attrs;
	wrapper->getWindowAttributes(display_, window_, &x_window_attrs);
	if (x_window_attrs.override_redirect)
		throw YggdrasilException("ignoring window with override redirect attribute.");
	this->frame_ = wrapper->createSimpleWindow(
			display_,
			root_,
			x_window_attrs.x,
			x_window_attrs.y,
			x_window_attrs.width,
			x_window_attrs.height,
			border_width,
			border_color,
			BG_COLOR
			);
	wrapper->selectInput(
			display_,
			this->frame_,
			SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask | ClientMessage);
	wrapper->addToSaveSet(display_,window_);
	wrapper->reparentWindow(
			display_,
			window_,
			frame_,
			0,0
			);
	wrapper->mapWindow(display_,frame_);
	//   a. Move windows with alt + left button.
	wrapper->grabButton(
			display_,
			Button1,
			Mod1Mask,
			window_,
			false,
			ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
			GrabModeAsync,
			GrabModeAsync,
			None,
			None);
	try {
		ConfigHandler::GetInstance().getConfigData<ConfigDataBindings>()->grabKeys(display_, window_);
	} catch (const std::exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
	this->framed = true;
//	this->group_->addClient(window_, this);
}
void Client::restack() {
	try {
		if (this->framed) {
			wrapper->raiseWindow(display_, frame_);
		}
		wrapper->raiseWindow(display_, window_);
	} catch (const X11Exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
void Client::unframe() {
	if (!this->framed)
		throw YggdrasilException("Client is not framed");
	try {
		wrapper->destroyWindow(display_,frame_);
	} catch (const std::exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
	this->framed = false;
	this->frame_ = 0;
}
Window Client::getWindow() const {
	return this->window_;
}
void Client::move(int x, int y) {
	try {
		if (this->framed) {
			wrapper->moveWindow(display_, frame_, x, y);
		} else {
			wrapper->moveWindow(display_, window_, x + (int) border_width / 2, y + (int) border_width / 2);
		}
	} catch (const X11Exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
void Client::resize(unsigned int width,unsigned int height) {
	try {
		if (this->framed) {
			wrapper->resizeWindow(display_, frame_, width, height);
		}
		wrapper->resizeWindow(display_, window_, width - border_width, height - border_width);
	} catch (const X11Exception &e) {
		Logger::GetInstance()->Log(e.what(), L_ERROR);
	}
}
Window Client::getFrame() const {return frame_; }
bool Client::isFramed() const {	return framed; }
bool Client::isMapped() const { return mapped; }
void Client::setMapped(bool m) { Client::mapped = m; }
const std::string &Client::getTitle() const { return title_; }
const std::string &Client::getClass() const { return class_; }
std::shared_ptr<Group>Client::getGroup() const { return group_.lock(); }
void Client::setGroup(std::shared_ptr<Group> g) { this->group_ = std::weak_ptr<Group>(g);}
