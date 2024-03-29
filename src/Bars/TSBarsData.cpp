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

#include "Bars/TSBarsData.hpp"
#include <condition_variable>
#include <mutex>

TSBarsData::TSBarsData() : mutex(), cv(), data(),modifiedKeys(){}
TSBarsData::~TSBarsData() {}
void TSBarsData::addData(std::string key, std::string value) {
	std::lock_guard<std::mutex> lock(mutex);
	data[key] = value;
	modifiedKeys.insert(key);
	dataChanged = true;
	cv.notify_one();
}
std::unordered_map<std::string,std::string> TSBarsData::getData() {
	std::unique_lock<std::mutex> lock(mutex);
	std::unordered_map<std::string, std::string> modifiedData;
	for (const auto& key : modifiedKeys) {
		modifiedData[key] = data[key]; // Assumes key still exists. Adjust if you track removals differently.
	}
	modifiedKeys.clear();
	return modifiedData;
}
void TSBarsData::removeData(std::string key) {
	std::lock_guard<std::mutex> lock(mutex);
	if (modifiedKeys.find(key) != modifiedKeys.end()) {
		modifiedKeys.erase(key);
	}
	data.erase(key);
	dataChanged = true;
	cv.notify_one();
}
void TSBarsData::modifyData(std::string key, std::string value) {
	std::lock_guard<std::mutex> lock(mutex);
	data[key] = value;
	modifiedKeys.insert(key);
	dataChanged = true;
	cv.notify_one();
}
bool TSBarsData::wait() {
	std::unique_lock<std::mutex> lock(mutex);
	if (!cv.wait_for(lock, std::chrono::milliseconds(1000),[this] { return dataChanged; }))
		return false;
	else {
		dataChanged = false;
		return true;
	}
}