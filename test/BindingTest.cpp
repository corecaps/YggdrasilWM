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
 * @file BindingTest.cpp
 * @brief BindingTest class
 * @date 2024-02-07
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Config/Binding.hpp"
#include "Logger.hpp"
#include <sstream>
#include "X11wrapper/baseX11Wrapper.hpp"
#include "X11wrapper/mockX11Wrapper.hpp"
extern "C" {
#include <X11/Xlib.h>
}
using ::testing::Return;
using ::testing::_;

class BindingTest : public ::testing::Test
{
protected:
	static std::ostringstream oss;
	mockX11Wrapper* x11WrapperMock;
	Display* displayMock;

	BindingTest() {
		x11WrapperMock = new mockX11Wrapper();
		displayMock = nullptr;
	}
	~BindingTest() override {
		delete x11WrapperMock;
	}
	static void SetUpTestSuite() {
		std::cout << " =================================================================================== " << std::endl;
		std::cout << " ============================= Binding SetUpTestSuite ============================== " << std::endl;
		std::cout << " =================================================================================== " << std::endl;
//		Logger::Create(BindingTest::oss,L_INFO);
		Logger::Create(std::cout,L_INFO);
	}
	void SetUp() override {

	}
	void TearDown() override {}
	static void TearDownTestSuite() {
		Logger::Destroy();
	}
};
std::ostringstream BindingTest::oss = std::ostringstream ();
TEST_F(BindingTest, constructor) {
	Binding binding;
	EXPECT_EQ(binding.getMod(), "");
	EXPECT_EQ(binding.getKey(), "");
	EXPECT_EQ(binding.getCommandName(), "");
	EXPECT_EQ(binding.getArgs(), "");
	EXPECT_EQ(binding.getModMask(), 0);
	EXPECT_EQ(binding.getKeyCode(), 0);
}

TEST_F(BindingTest, init) {

	Binding binding;
	std::string mod = "Mod4";
	std::string key = "Return";
	std::string command = "FocusGroup";
	std::string args = "1";

	EXPECT_CALL(*this->x11WrapperMock, keysymToKeycode(displayMock, _))
			.Times(1)
			.WillOnce(Return(1));
	EXPECT_CALL(*this->x11WrapperMock, stringToKeysym(_))
			.Times(1)
			.WillOnce(Return(1));
	binding.init(mod, key, command, args);
	binding.init_keycode(nullptr, this->x11WrapperMock);
	EXPECT_EQ(binding.getMod(), mod);
	EXPECT_EQ(binding.getKey(), key);
	EXPECT_EQ(binding.getCommandName(), command);
	EXPECT_EQ(binding.getArgs(), args);
	EXPECT_EQ(binding.getModMask(), Mod4Mask);
	EXPECT_EQ(binding.getKeyCode(), 1);
}

TEST_F(BindingTest, wrongInit) {
	Binding binding1;
	std::string mod = "InvalidMod";
	std::string key = "Return";
	std::string command = "FocusGroup";
	std::string args = "1";
	EXPECT_THROW(binding1.init(mod, key, command, args), std::runtime_error);
	Binding binding2;
	mod = "Mod4";
	command = "InvalidCommand";
	EXPECT_THROW(binding2.init(mod, key, command, args), std::runtime_error);
	Binding binding3;
	EXPECT_THROW(binding3.execute(), std::runtime_error);
}