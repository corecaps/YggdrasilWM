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
#ifndef TSBARSDATA_HPP
#define TSBARSDATA_HPP
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <string>
#include <set>
class TSBarsData
{
public:
	TSBarsData();
	~TSBarsData();
	void addData(std::string key, std::string value);
	std::unordered_map<std::string,std::string> getData();
	void removeData(std::string key);
	void modifyData(std::string key, std::string value);
	bool wait();
	bool dataChanged = false;
private:
	std::mutex										mutex;
	std::condition_variable							cv;
	std::unordered_map<std::string, std::string>	data;
	std::set<std::string>							modifiedKeys;
};
#endif // TSBARSDATA_HPP