/**
 * @file Client.hpp
 * @brief Client class header.
 * @date 2021-06-23
 * This class is responsible for managing the client windows.
 */
#ifndef YGGDRASILWM_CLIENT_HPP
#define YGGDRASILWM_CLIENT_HPP
#include "window_manager.hpp"

class Client {
public:
	Client(const WindowManager &wm,const Logger &logger, Window window);
	~Client();
	void focus();
	void unfocus();
	void move(int x, int y);
	void resize(int width, int height);
	void set_border_color(unsigned long color);
	void set_border_width(unsigned int width);

private:
	const WindowManager& wm_;
	const Logger& logger_;
	Window window;
	Window frame;
	unsigned int border_width;
	unsigned long border_color;
	int x, y;
	int width, height;
	bool focused;
};

#endif //YGGDRASILWM_CLIENT_HPP
