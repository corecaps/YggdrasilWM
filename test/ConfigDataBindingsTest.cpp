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
 * @file ConfigDataBindingsTest.cpp
 * @brief ConfigDataBindings class unit tests.
 * @date 2024-02-07
 *
 */

#include <gtest/gtest.h>
#include "Config/ConfigDataBindings.hpp"
#include <gmock/gmock.h>
#include "Logger.hpp"
#include <sstream>
#include "json/json.h"
#include "X11wrapper/mockX11Wrapper.hpp"
using ::testing::TestWithParam;
using ::testing::Values;

class ConfigDataBindingsTest :  public ::testing::Test {
protected:
	static std::ostringstream oss;
	ConfigDataBindings *cdb;
	ConfigDataBindingsTest() {}
	~ConfigDataBindingsTest() override = default;
	static void SetUpTestSuite() {
		std::cout << " =================================================================================== " << std::endl;
		std::cout << " ========================== ConfigDataBindings SetUpTestSuite ========================== " << std::endl;
		std::cout << " =================================================================================== " << std::endl;
		Logger::Create(ConfigDataBindingsTest::oss,L_INFO);
	}

	void SetUp() override {
		cdb = new ConfigDataBindings();
	}
	void TearDown() override {
		delete cdb;
		cdb = nullptr;
	}
	static void TearDownTestSuite() {
		Logger::Destroy();
	}
};

std::ostringstream ConfigDataBindingsTest::oss = std::ostringstream ();
std::string configPath = "ConfigDataBindingTest.json";
TEST_F(ConfigDataBindingsTest, CreateConfigDataBindings) {
	ASSERT_NE(cdb, nullptr);
}
class ConfigDataBindingsParamTest : public ::testing::TestWithParam<Json::Value> {
protected:
	static std::ostringstream oss;
	ConfigDataBindings* cdb;
	static void SetUpTestSuite() {
		Logger::Create(ConfigDataBindingsParamTest::oss,L_INFO);
	}
	void SetUp() override {
		cdb = new ConfigDataBindings();
	}
	void TearDown() override {
		delete cdb;
		cdb = nullptr;
	}
	static void TearDownTestSuite() {
		Logger::Destroy();
	}
};
std::ostringstream ConfigDataBindingsParamTest::oss = std::ostringstream ();
Json::Value makeBindingJson(std::string configJson) {
	Json::Value root;
	Json::Reader reader;
	reader.parse(configJson, root);
	return root;
}
INSTANTIATE_TEST_SUITE_P(
		ConfigDataBindingsTests,
		ConfigDataBindingsParamTest,
		::testing::Values(
				makeBindingJson("{\"Mod1\":[{\"Key\":\"1\",\"Action\":\"FocusGroup\",\"Argument\":\"1\"}, {\"Key\":\"2\",\"Action\":\"FocusGroup\",\"Argument\":\"2\"}, {\"Key\":\"3\",\"Action\":\"FocusGroup\",\"Argument\":\"3\"}, {\"Key\":\"Return\",\"Action\":\"Spawn\",\"Argument\":\"kitty\"}, {\"Key\":\"Q\",\"Action\":\"Quit\",\"Argument\":\"\"}]}"),
				makeBindingJson("{\"Mod1\":[{\"Key\":\"1\",\"Action\":\"FocusGroup\",\"Argument\":\"1\"}, {\"Key\":\"2\",\"Action\":\"FocusGroup\",\"Argument\":\"2\"}],\"Mod4\": [{\"Key\":\"3\",\"Action\":\"FocusGroup\",\"Argument\":\"3\"}, {\"Key\":\"Return\",\"Action\":\"Spawn\",\"Argument\":\"kitty\"}, {\"Key\":\"Q\",\"Action\":\"Quit\",\"Argument\":\"\"}]}"),
				makeBindingJson("")
				));
TEST_P (ConfigDataBindingsParamTest, LoadConfigDataBindings) {
	Json::Value json = GetParam();
	int correctBindings = 0;
	if (json.empty()) {
		ASSERT_NO_THROW(cdb->configInit(json));
		ASSERT_EQ(cdb->getBindings().size(), 0);
	} else {
		ASSERT_NO_THROW(cdb->configInit(json));
		ASSERT_NE(cdb->getBindings().size(), 0);
		for (auto modKey : json.getMemberNames()) {
			for (auto binding: json[modKey]) {
				std::string key = binding["Key"].asString();
				std::string command = binding["Action"].asString();
				std::string arg = binding["Argument"].asString();
				bool found = false;
				for (auto &b: cdb->getBindings()) {
					if (b->getMod() == modKey && b->getKey() == key && b->getCommandName() == command &&
						b->getArgs() == arg) {
						found = true;
						correctBindings++;
						break;
					}
				}
				ASSERT_TRUE(found);
			}
		}
		std::cout << "Correct bindings: " << correctBindings << std::endl;
	}
}