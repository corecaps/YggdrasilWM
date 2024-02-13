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
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * @file ConfigDataBarTest.cpp
 * @brief ConfigDataBar class unit tests.
 * @date 2024-02-07
 *
 */

 #include <gtest/gtest.h>
#include "Config/ConfigDataBar.hpp"
#include "json/json.h"
#include "Logger.hpp"
#include <sstream>
class ConfigDataBarTest : public ::testing::Test {
public:
	static std::ostringstream oss;
protected:
	static void SetUpTestSuite() {
		std::cout << " =================================================================================== " << std::endl;
		std::cout << " ======================== ConfigDataBar Tests Set up =============================== " << std::endl;
		std::cout << " =================================================================================== " << std::endl;
	}
	void SetUp() override {
		Logger::Create(ConfigDataBarTest::oss, L_INFO);
	}
	void TearDown() override {
		Logger::Destroy();
	}
};
std::ostringstream ConfigDataBarTest::oss = std::ostringstream();
TEST_F(ConfigDataBarTest, Initialization) {
	ConfigDataBar bar;
	EXPECT_NE(&bar, nullptr);
	EXPECT_EQ(bar.getBarHeight(), 20);
	EXPECT_EQ(bar.getBarFont(), "Arial");
	EXPECT_EQ(bar.getBarFontSize(), 12);
	EXPECT_EQ(bar.getBarFontColor(), 0x00ff00);
	EXPECT_EQ(bar.getBarBackgroundColor(), 0x00ff00);
	EXPECT_EQ(bar.getBarBorderSize(), 2);
}
TEST_F(ConfigDataBarTest, ConfigInitialization) {
	Json::Value root;
	root["Height"] = 20; // Example configuration
	root["Font"] = "Arial";
	root["Border_Size"] = 2;
	root["Font_Size"] = 12;
	root["Font_Color"] = "#00ff00";
	root["Background_Color"] = "#ff0000";
	root["Border_Color"] = "#0000ff";
	ConfigDataBar bar;
	bar.configInit(root);
	EXPECT_EQ(bar.getBarHeight(), 20);
	EXPECT_EQ(bar.getBarFont(), "Arial");
	EXPECT_EQ(bar.getBarFontSize(), 12);
	EXPECT_EQ(bar.getBarFontColor(), 0x00ff00);
	EXPECT_EQ(bar.getBarBackgroundColor(), 0xff0000);
	EXPECT_EQ(bar.getBarBorderColor(), 0x0000ff);
}
TEST_F(ConfigDataBarTest, ConfigInitializationInvalid) {
	Json::Value root;
	ConfigDataBar bar;
	EXPECT_THROW(bar.configInit(root), std::runtime_error);
}