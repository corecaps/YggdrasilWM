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
 */
#ifndef BARS_HPP
#define BARS_HPP
#include <vector>
#include <memory>
extern "C" {
#include <X11/Xlib.h>
}

class Bar;
class ConfigDataBars;
class TSBarsData;

class Bars
{
public:
	Bars(const Bars&) = delete;
	Bars& operator=(const Bars&) = delete;
	static void createInstance();
	static Bars& getInstance();
	static void destroy();
	~Bars();
	void init();
	void run();
	void redraw();
	unsigned int getSpaceN() const;
	unsigned int getSpaceS() const;
	unsigned int getSpaceE() const;
	unsigned int getSpaceW() const;
private:
	Bars();
	static Bars* instance;
	std::vector<std::unique_ptr<Bar>> bars;
	ConfigDataBars* configData;
	TSBarsData* tsData;
	Display* display;
	Window* root;
	unsigned int spaceN;
	unsigned int spaceS;
	unsigned int spaceE;

	unsigned int spaceW;
	void selectEvents();
};
#endif // BARS_HPP