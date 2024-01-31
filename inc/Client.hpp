/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
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

#include <string>

class TreeLayoutManager;
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
	Client(Display *display, Window root, Window window, TreeLayoutManager *layout_manager);
	~Client();
	Client_Err frame();
	Client_Err unframe();
	Window getWindow() const;
	static std::string getError(Client_Err error);
	void focus();
	void unfocus();
	void move(int x, int y);
	void resize(int width, int height);
	void set_border_color(unsigned long color);
	void set_border_width(unsigned int width);
	bool isFramed() const;
	void setMapped(bool mapped);
	bool isMapped() const;
	const std::string &getTitle() const;
	const std::string &getClass() const;
	Window getFrame() const;
	bool isFocused() const;
	void setFocused(bool focused);

private:
	Display *display_;
	TreeLayoutManager *layout_manager_;
	Window root_;
	Window window_;
	Window frame_;
	unsigned int border_width;
	unsigned long border_color;
	bool focused;
	bool framed;
	bool mapped{};
	std::string title_;
	std::string class_;
};

#endif //YGGDRASILWM_CLIENT_HPP
