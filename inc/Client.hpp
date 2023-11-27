/**
 * @file Client.hpp
 * @brief  This class is responsible for managing the client windows.
 * @date 2021-06-23
 *
 */
#ifndef YGGDRASILWM_CLIENT_HPP
#define YGGDRASILWM_CLIENT_HPP
extern "C" {
#include <X11/Xlib.h>
}

enum Client_Err {
	YGG_CLI_NO_ERROR,
	YGG_CLI_LOG,
	YGG_CLI_LOG_ALREADY_FRAMED,
	YGG_CLI_LOG_IGNORED_OVERRIDE_REDIRECT,
	YGG_CLI_LOG_IGNORE_NOT_FRAMED,
	YGG_CLI_WARNING,
	YGG_CLI_ERROR,
	YGG_CLI_ERR_RETRIEVE_ATTR,

};
class Client {
public:
	Client(Display * display, Window root, Window window);
	~Client();
	Client_Err	frame();
	Client_Err	unframe();
	Window		getWindow();
	void		focus();
	void		unfocus();
	void		move(int x, int y);
	void		resize(int width, int height);
	void		set_border_color(unsigned long color);
	void		set_border_width(unsigned int width);

private:
	Display*				display_;
	Window					root_;
	Window					window_;
	Window					frame_;
	unsigned int			border_width;
	unsigned long			border_color;
	bool					focused;
	bool					framed;
};

#endif //YGGDRASILWM_CLIENT_HPP
