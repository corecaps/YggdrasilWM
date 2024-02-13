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
 * @file ConfigDataBarsTest.cpp
 * @brief ConfigDataBarsTest class
 * @date 2024-02-07
 *
 */

#include <gtest/gtest.h>
#include "Config/ConfigDataBars.hpp"
#include "Config/ConfigDataBar.hpp"
#include "Logger.hpp"
#include <sstream>
#include "json/json.h"

class ConfigDataBarsTest : public ::testing::Test
{
protected:
	static std::ostringstream oss;
	ConfigDataBarsTest() {}
	~ConfigDataBarsTest() override = default;
	static void SetUpTestSuite() {
		std::cout << " =================================================================================== " << std::endl;
		std::cout << " ========================== ConfigDataBars SetUpTestSuite ========================== " << std::endl;
		std::cout << " =================================================================================== " << std::endl;
		Logger::Create(ConfigDataBarsTest::oss,L_INFO);
	}
	void SetUp() override {}
	void TearDown() override {}
	static void TearDownTestSuite() {
		Logger::Destroy();
	}
};
std::ostringstream ConfigDataBarsTest::oss = std::ostringstream ();

TEST_F(ConfigDataBarsTest, CreateConfigDataBars) {
	auto *cdb = new ConfigDataBars();
	ASSERT_NE(cdb, nullptr);
	delete cdb;
}

TEST_F(ConfigDataBarsTest, configInit) {
	auto *cdb = new ConfigDataBars();
	Json::Value bar1;
	bar1["Height"] = 20;
	bar1["Font"] = "monospace";
	bar1["FontSize"] = 12;
	bar1["Font_Color"] = "#ffffff";
	bar1["Background_Color"] = "#000000";
	bar1["Border_Size"] = 2;
	bar1["Border_Color"] = "#000000";
	Json::Value root;
	root.append(bar1);
	cdb->configInit(root);
	ASSERT_EQ(cdb->getBars().size(), 1);
	ASSERT_EQ(cdb->getBars()[0]->getBarHeight(), 20);
	ASSERT_EQ(cdb->getBars()[0]->getBarFont(), "monospace");
	ASSERT_EQ(cdb->getBars()[0]->getBarFontSize(), 12);
	ASSERT_EQ(cdb->getBars()[0]->getBarFontColor(), 0xffffff);
	ASSERT_EQ(cdb->getBars()[0]->getBarBackgroundColor(), 0x000000);
	ASSERT_EQ(cdb->getBars()[0]->getBarBorderSize(), 2);
	ASSERT_EQ(cdb->getBars()[0]->getBarBorderColor(), 0x000000);
	delete cdb;
}

TEST_F(ConfigDataBarsTest, configInitEmpty) {
	auto *cdb = new ConfigDataBars();
	Json::Value root;
	EXPECT_ANY_THROW(cdb->configInit(root));
	delete cdb;
}

TEST_F(ConfigDataBarsTest, configInitEmptyBar) {
	auto *cdb = new ConfigDataBars();
	Json::Value bar1;
	Json::Value root;
	root.append(bar1);
	EXPECT_ANY_THROW(cdb->configInit(root));
	delete cdb;
}
TEST_F(ConfigDataBarsTest,configInitNotAnArray) {
	auto *cdb = new ConfigDataBars();
	Json::Value bar1;
	Json::Value root;
	root["bar1"] = bar1;
	EXPECT_ANY_THROW(cdb->configInit(root));
	delete cdb;
}
TEST_F(ConfigDataBarsTest, configInvalidData) {
	auto *cdb = new ConfigDataBars();
	Json::Value bar1;
	bar1["Font"] = "monospace";
	bar1["FontSize"] = 12;
	bar1["Font_Color"] = "ffffff";
	bar1["Background_Color"] = "#000000";
	bar1["Border_Size"] = 2;
	bar1["Border_Color"] = "#000000";
	Json::Value root;
	root.append(bar1);
	EXPECT_ANY_THROW(cdb->configInit(root));
	delete cdb;
}