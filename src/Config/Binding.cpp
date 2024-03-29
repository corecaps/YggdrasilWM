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
 * @file Binding.cpp
 * @brief Binding class implementation.
 * @date 2024-02-11
 */
#include <string>
#include "Config/Binding.hpp"
#include "Logger.hpp"
#include "Commands/FocusGroup.hpp"
#include "Commands/Spawn.hpp"
#include "Commands/Quit.hpp"
#include "Commands/Grow.hpp"
#include "WindowManager.hpp"
#include "X11wrapper/baseX11Wrapper.hpp"
extern "C" {
#include <X11/Xlib.h>

}

void Binding::init(std::string Mod, std::string Key, std::string Command, std::string Args) {
	mod_ = Mod;
	key_ = Key;
	commandName_ = Command;
	args_ = Args;
	if (commandName_ == "FocusGroup") {
		command_ = new FocusGroup();
	} else if (commandName_ == "Spawn") {
		command_ = new Spawn();
	} else if (commandName_== "Quit") {
		command_ = new Quit();
	} else if (commandName_ == "Grow") {
		 command_ = new Grow();
	} else {
		throw std::runtime_error("Unknown command: " + commandName_);
	}
	if (mod_ == "Mod4") {
		modMask_ = Mod4Mask;
	} else if (mod_ == "Mod1") {
		modMask_ = Mod1Mask;
	} else {
		throw std::runtime_error("Unknown mod : " + mod_);
	}
}
void Binding::execute(const XKeyEvent *event = nullptr) {
	if (command_ != nullptr) {
		if (event == nullptr) {
			command_->execute(args_);
		} else {
			std::stringstream ss;
			ss << args_ << "," <<event->subwindow << "," << event->window << "," << event->state << "," << event->keycode;
			command_->execute(ss.str());
		}
	}
	else
		throw std::runtime_error("Command not initialized.");
}
Binding::Binding() :
	mod_(""),
	modMask_(0),
	key_(""),
	keyCode_(0),
	commandName_(""),
	args_(""),
	command_(nullptr) {}
const std::string &Binding::getMod() const { return mod_; }
const std::string &Binding::getKey() const { return key_; }
const std::string &Binding::getCommandName() const { return commandName_; }
const std::string &Binding::getArgs() const { return args_; }
unsigned int Binding::getModMask() const { return modMask_; }
int Binding::getKeyCode() const { return keyCode_; }

void Binding::init_keycode(Display *display, BaseX11Wrapper *x11Wrapper) {
	keyCode_ = x11Wrapper->keysymToKeycode(display, (int)x11Wrapper->stringToKeysym(key_.c_str()));
	Logger::GetInstance()->Log("Binding registered : ["
								+ mod_
								+ " + "
								+key_
								+"] Command: "
								+ commandName_
								+ " Args: "
								+ args_
								+ " keycode: ["
								+ std::to_string(keyCode_)
								+"]", L_INFO);
}

Binding::~Binding() {
	delete command_;
}
