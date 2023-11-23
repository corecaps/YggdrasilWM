//
// Created by corecaps on 11/22/23.
//

#ifndef YGGDRASILWM_CLIENT_HPP
#define YGGDRASILWM_CLIENT_HPP
#include "window_manager.hpp"

class Client {
public:
	Client(WindowManager *window_manager, Window window);
	~Client();
	void focus();
	void unfocus();
	void move(int x, int y);
	void resize(int width, int height);
	void set_border_color(unsigned long color);
	void set_border_width(unsigned int width);
	void set_border_width(unsigned int width);
	void set_border_color(unsigned long color);
private:
	WindowManager *window_manager;
	Window window;
	unsigned int border_width;
	unsigned long border_color;
	int x, y;
	int width, height;
	bool focused;
};

#endif //YGGDRASILWM_CLIENT_HPP
