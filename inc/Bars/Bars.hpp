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
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * @file Bars.hpp
 * @brief Bars class header file
 */
#ifndef BARS_HPP
#define BARS_HPP
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
extern "C" {
#include <X11/Xlib.h>
}

class Bar;
class ConfigDataBars;
class TSBarsData;

/**
 * @class Bars
 * @brief Class to manage the bars
 * This class is a singleton and is responsible for managing the bars
 * instanciate with createInstance and initialize with init
 * should be destroyed with destroy when the program ends
 * Bars::run runs in a separate thread, so all data must be passed
 * through the TSBarsData class that handle synchronization
 * @note This class is a singleton
 * @see TSBarsData
 */
class Bars
{
public:
	Bars(const Bars&) = delete;
	Bars& operator=(const Bars&) = delete;
/**
 * @fn static void Bars::createInstance()
 * @brief Create the singleton instance
 * if the instance is already created, it will do nothing
 */
	static void createInstance();
/**
 * @fn static Bars& Bars::getInstance()
 * @brief Get the singleton instance
 * if the instance is not created, it will create it
 * @return reference to the singleton instance
 */
	static Bars& getInstance();
/**
 * @fn static void Bars::destroy()
 * @brief Destroy the singleton instance
 * if the instance is not created, it will do nothing
 */
	static void destroy();
	~Bars();
/**
 * @fn void Bars::init(ConfigDataBars *configData, TSBarsData *tsData, Display *display, Window *root)
 * @brief Initialize the Bars class
 * @param configData global bars configuration will pass the right config data to each bar
 * @param tsData thread safe data to pass data between threads
 * @param display to limit calls to WindowManager::getDisplay
 * @param root to limit calls to WindowManager::getRoot
 */
	void init(ConfigDataBars *configData,
			  TSBarsData *tsData,
			  Display *display,
			  Window root);
/**
 * @fn void Bars::run()
 * @brief Run the Bars class in a separate thread
 * @note all data must be passed through the TSBarsData class
 */
	void run();
	void start_thread();
/**
 * @fn unsigned int Bars::getSpaceN() const
 * @brief Get the spaceN value
 * used to calculate the space left for the Layout Manager.
 */
	[[nodiscard]] unsigned int getSpaceN() const;
/**
 * @fn unsigned int Bars::getSpaceS() const
 * @brief Get the spaceS value
 * used to calculate the space left for the Layout Manager.
 */
	[[nodiscard]] unsigned int getSpaceS() const;
/**
 * @fn unsigned int Bars::getSpaceE() const
 * @brief Get the spaceE value
 * used to calculate the space left for the Layout Manager.
 */
	[[nodiscard]] unsigned int getSpaceE() const;
/**
 * @fn unsigned int Bars::getSpaceW() const
 * @brief Get the spaceW value
 * used to calculate the space left for the Layout Manager.
 */
	[[nodiscard]] unsigned int getSpaceW() const;
	const std::vector<Window> &getWindows() const;
	const std::unordered_map<std::string, std::string> &getData() const;
/**
 * @fn void Bars::redraw()
 * @brief Redraw the bars by calling the draw method of each bar
 */
	void redraw();
private:
	static Bars* instance;
	std::vector<std::unique_ptr<Bar>> bars;
	std::vector<Window> windows;
	ConfigDataBars *configData;
	TSBarsData* tsData;
	std::unordered_map<std::string, std::string> data;
	Display* display;
	Window root;
	unsigned int spaceN;
	unsigned int spaceS;
	unsigned int spaceE;
	unsigned int spaceW;

	Bars();
/**
 * @fn void Bars::selectEvents()
 * @brief Select the events for the bars
 */
	void selectEvents();

};
#endif // BARS_HPP