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
 * @file Grow.cpp
 * @brief Grow class implementation.
 * @date 2024-02-12
 */
#include "Commands/Grow.hpp"
#include "WindowManager.hpp"
#include "Group.hpp"
#include "Logger.hpp"


Grow::Grow() {

}

void Grow::execute(const std::string &args) {
	WindowManager *wm = WindowManager::getInstance();
	DeserializedXKeyEvent event = deserializeXKeyEvent(args);
	std::shared_ptr<Client> client = wm->getClient(event.subwindow);
	if (client) {
		LayoutManager * lm = wm->getActiveGroup()->getLayoutManager().get();
		TreeLayoutManager * tlm = dynamic_cast<TreeLayoutManager *>(lm);
		if (tlm) {
			tlm->growSpace(client.get(), event.inc);
		}
	}
}

DeserializedXKeyEvent Grow::deserializeXKeyEvent(const std::string &serializedEvent) {
	DeserializedXKeyEvent event;
	std::istringstream ss(serializedEvent);
	std::string item;

	// Extract args_
	std::getline(ss, item, ',');
	event.inc = std::stoi(item);

	// Extract subwindow
	std::getline(ss, item, ',');
	event.subwindow = static_cast<Window>(std::stoul(item));

	// Extract window
	std::getline(ss, item, ',');
	event.window = static_cast<Window>(std::stoul(item));

	// Extract state
	std::getline(ss, item, ',');
	event.state = std::stoul(item);

	// Extract keycode
	std::getline(ss, item, ',');
	event.keycode = std::stoul(item);

	return event;
}
