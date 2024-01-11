/**
 * @file window_manager.hpp
 * @brief WindowManager class header.
 * @date 2021-06-23
 * This class is responsible for managing the windows.in an X11 session.
 * It is the main class of the project.
 * It should be instantiated only via the static Create function.
 * The Run function is the main loop of the program.
 * @see Create
 * @see Run
 * @see Client
 * @see Logger
 * @see EventHandler
 * @see ConfigHandler
 * @see LayoutManager
 */

#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP
extern "C" {
#include <X11/Xlib.h>
}
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include "util.hpp"
#include "Logger.hpp"
#include "Client.hpp"

class WindowManager {
public:
	static ::std::unique_ptr<WindowManager> Create(
	const Logger &logger,
	const std::string &display_str = std::string());
	~WindowManager();
	void Init();
	void Run();
	const Logger &getLogger() const;
	Display *getDisplay() const;
	std::unordered_map<Window, Client *> &getClients();
	const Window getRoot() const;
	void insertClient(Window window);
	Client &getClient(Window window);
	bool isFrame(Window window);

private:
	static bool								wm_detected_;
	const Window							root_;
	const Logger&							logger_;
	Display									*display_;
	static ::std::mutex						wm_detected_mutex_;
	Position<int>							drag_start_pos_;
	Position<int>							drag_start_frame_pos_;
	Size<int>								drag_start_frame_size_;
	const Atom								WM_PROTOCOLS;
	const Atom								WM_DELETE_WINDOW;
	std::unordered_map<Window, Client*>		clients_;
	bool									running;

	void selectEventOnRoot() const;
	WindowManager(Display *display, const Logger &logger);
	static int OnXError(Display *display, XErrorEvent *e);
	static int OnWMDetected(Display *display, XErrorEvent *e);
	void getTopLevelWindows(std::stringstream &debug_stream);

};

#endif
