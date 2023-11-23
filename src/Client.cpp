/**
 * @file Client.cpp
 * @brief Client class implementation.
 * @date 2021-06-23
 *
 */

#include "Client.hpp"

Client::Client(const WindowManager &wm, const Logger &logger, Window window)
	:	wm_(wm),
		logger_(logger),
		window(window),
		frame(0),
		border_width(0),
		border_color(0),
		x(0), y(0),
		width(0), height(0),
		focused(false) {

}
Client::~Client() {}
