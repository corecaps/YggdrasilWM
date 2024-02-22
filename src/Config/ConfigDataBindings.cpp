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
 * @file ConfigDataBindings.cpp
 * @brief ConfigDataBindings class implementation.
 * @date 2024-02-12
 */

#include "Config/ConfigDataBindings.hpp"
#include "Logger.hpp"

ConfigDataBindings::ConfigDataBindings() : bindings_(){}
void ConfigDataBindings::configInit(const Json::Value &root) {
	std::vector<std::string> modKeys = root.getMemberNames();
	for (auto &modKey : modKeys) {
		Json::Value bindings = root[modKey];
		for (auto &binding : bindings) {
			std::string key = binding["Key"].asString();
			std::string command = binding["Action"].asString();
			std::string arg = binding["Argument"].asString();
			auto b = new Binding();
			b->init(modKey, key, command, arg);
			bindings_.push_back(b);
		}
	}
	Logger::GetInstance()->Log("Successfully added " + std::to_string(bindings_.size()) + " bindings", L_INFO);
}
Json::Value ConfigDataBindings::configSave() {
	return Json::Value();
}
ConfigDataBindings::~ConfigDataBindings() {
	for (auto &binding : bindings_) {
		delete binding;
	}
}

void ConfigDataBindings::grabKeys(Display *display, Window window) {
	for (auto &binding : bindings_) {
		XGrabKey(display, binding->getKeyCode(), binding->getModMask(), window, true, GrabModeAsync, GrabModeAsync);
	}
	XFlush(display);
}

void ConfigDataBindings::handleKeypressEvent(const XKeyEvent *event) {
	unsigned int keyCode = event->keycode;
	bool modOk = false;
	for (auto &binding : bindings_) {
		modOk = event->state & binding->getModMask();
		if (binding->getKeyCode() == keyCode && modOk) {
			binding->execute();
			Logger::GetInstance()->Log("[" +
										binding->getMod()
										+ "+"
										+ binding->getKey()
										+"] ->\t"
										+ binding->getCommandName()
										+ " ["
										+ binding->getArgs()
										+ "]",L_INFO);
		}
	}
}

void ConfigDataBindings::initKeycodes(Display *display) {
	for (auto &binding : bindings_) {
		binding->init_keycode(display);
	}
}
