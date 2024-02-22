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

#include <X11/keysym.h>
#include <iostream>
#include <cstring>
#include "Client.hpp"
#include "Layouts/LayoutManager.hpp"
#include "Group.hpp"
#include "Logger.hpp"
#include "Config/ConfigHandler.hpp"
#include "Config/ConfigDataBindings.hpp"

Client::Client(Display *display,
			   Window root,
			   Window window,
			   Group *group,
			   unsigned long inActiveColor,
			   int borderSize)
		: display_(display),
		  root_(root),
		  group_(group),
		  window_(window),
		  frame_(0),
		  border_width(borderSize),
		  border_color(inActiveColor),
		  framed(false),
		  mapped(false)
{
	
	Atom wmClassAtom = XInternAtom(display, "WM_CLASS", False);
	if (wmClassAtom == None) {
		Logger::GetInstance()->Log("Failed to intern WM_CLASS atom.",L_ERROR);
	}
	Atom actualType;
	int actualFormat;
	unsigned long nItems, bytesAfter;
	unsigned char* propData;
	if (XGetWindowProperty(display, window, wmClassAtom, 0, 1024, False, AnyPropertyType,
						   &actualType, &actualFormat, &nItems, &bytesAfter, &propData) != Success) {
		Logger::GetInstance()->Log("Failed to get WM_CLASS property.", L_ERROR);
	}
	Atom XA_STRING = XInternAtom(display, "STRING", False);
	if (actualType == XA_STRING && actualFormat == 8 && nItems > 1) {
		// The WM_CLASS property contains two null-terminated strings.
		char* instanceName = reinterpret_cast<char*>(propData);
		char* className = instanceName + strlen(instanceName) + 1;
		this->class_ = className;
		this->title_ = instanceName;
	} else {
		Logger::GetInstance()->Log("Unexpected format or size of WM_CLASS property.",L_ERROR);
	}
	XFree(propData);
}
Client::~Client() {
	if (this->framed) {
		XDestroyWindow(display_, frame_);
	}
	Logger::GetInstance()->Log("Client destroyed :" + title_, L_INFO);
}
Client_Err Client::frame() {
	const unsigned long BG_COLOR = 0x000000;
	border_width = group_->getBorderSize();
	border_color = group_->getInactiveColor();
	if (this->framed)
		return(YGG_CLI_LOG_ALREADY_FRAMED);
	XWindowAttributes x_window_attrs;
	if (XGetWindowAttributes(display_, window_, &x_window_attrs) == 0)
		return (YGG_CLI_ERR_RETRIEVE_ATTR);
	if (x_window_attrs.override_redirect)
		return(YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT);
	this->frame_ = XCreateSimpleWindow(
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
		XSelectInput(
			display_,
			this->frame_,
			SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask | ClientMessage);
	XAddToSaveSet(display_,window_);
	XReparentWindow(
			display_,
			window_,
			frame_,
			0,0
			);
	XMapWindow(display_,frame_);
	//   a. Move windows with alt + left button.
	XGrabButton(
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
	ConfigHandler::GetInstance().getConfigData<ConfigDataBindings>()->grabKeys(display_, window_);
	this->framed = true;
	this->group_->addClient(window_, this);
	return YGG_CLI_NO_ERROR;
}
void Client::restack() {
	if (this->framed) {
		XRaiseWindow(display_, frame_);
	}
	XRaiseWindow(display_, window_);
}
Client_Err Client::unframe() {
	if (!this->framed)
		return(YGG_CLI_LOG_IGNORE_NOT_FRAMED);
	XDestroyWindow(display_,frame_);
	this->framed = false;
	this->frame_ = 0;
	return YGG_CLI_NO_ERROR;
}
Window Client::getWindow() const {
	return this->window_;
}
void Client::move(int x, int y) {
	if (this->framed) {
		XMoveWindow(display_, frame_, x, y);
	}
	else {
		XMoveWindow(display_, window_, x + (int)border_width / 2, y + (int)border_width / 2);
	}
}
void Client::resize(unsigned int width,unsigned int height) {
	if (this->framed) {
		XResizeWindow(display_, frame_, width, height);
	}
	XResizeWindow(display_, window_, width - border_width , height - border_width);
}
std::string Client::getError(Client_Err error) {
	switch (error) {
		case YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT:
			return "Client::Frame ignoring window with override redirect attribute.";
		case YGG_CLI_LOG_ALREADY_FRAMED :
			return "Client::Frame Client has already been framed";
		case YGG_CLI_ERR_RETRIEVE_ATTR:
			return "Client could not retrieve Window Attributes";
		case YGG_CLI_LOG_IGNORE_NOT_FRAMED:
			return"Client::unFrame Client has no frame";
		default:
			return "Unknown error from Client Class";
	}
}
Window Client::getFrame() const {return frame_; }
bool Client::isFramed() const {	return framed; }
bool Client::isMapped() const { return mapped; }
void Client::setMapped(bool m) { Client::mapped = m; }
const std::string &Client::getTitle() const { return title_; }
const std::string &Client::getClass() const { return class_; }
Group *Client::getGroup() const { return group_; }
