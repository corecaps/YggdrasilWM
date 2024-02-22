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
 * @file ConfigDataBindings.hpp
 * @brief ConfigDataBindings class header.
 * @date 2024-02-12
 */

#ifndef YGGDRASILWM_CONFIGDATABINDINGS_HPP
#define YGGDRASILWM_CONFIGDATABINDINGS_HPP
#include <string>
#include <vector>
extern "C" {
#include <X11/Xlib.h>
}
#include "json/json.h"
#include "ConfigDataBase.hpp"
#include "Config/Binding.hpp"
/**
 * @class ConfigDataBindings
 * @brief ConfigDataBindings class.
 * This class is used to handle the bindings configuration
 * it instanciates the bindings in a vector
 * it handle grabbing keys and handle keypress events
 */
class ConfigDataBindings : public ConfigDataBase {
public:
	ConfigDataBindings();
	~ConfigDataBindings() override;
/**
 * @fn void ConfigDataBindings::configInit(Json::Value& root_)
 * @brief Parse the Json::Value object to initialize the bindings
 * Instanciate the bindings in a vector
 * @param root_ the Json::Value object containing the bindings
 */
	void configInit(const Json::Value& root_) override;
	void initKeycodes(Display *display);
	[[nodiscard]] Json::Value configSave() override;
/**
 * @fn void ConfigDataBindings::grabKeys(Display *display, Window window)
 * @brief Grab the keys from the bindings
 * GrabKeys is called on the root window on initialisation of the
 * WindowManager and then on each new window creation that is managed
 * by the WindowManager
 * @param display
 * @param window
 */
	void grabKeys(Display *display, Window window);
/**
 * @fn void ConfigDataBindings::handleKeypressEvent(const XKeyEvent *event)
 * @brief Handle the keypress event
 * This method is called by the EventManager to avoid handling
 * bindings and their interface in the EventHandle class.
 * @param event must be converted to const XKeyEvent *event
 */
	void handleKeypressEvent(const XKeyEvent *event);

private:
	std::vector<Binding *> bindings_;
};
#endif //YGGDRASILWM_CONFIGDATABINDINGS_HPP