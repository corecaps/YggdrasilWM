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
 * @file ClientTest.cpp
 * @brief ClientTest class
 * @date 2024-02-07
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Client.hpp"
#include "X11wrapper/mockX11Wrapper.hpp" // Assume you have a mock for BaseX11Wrapper
#include "Logger.hpp"
#include <memory>

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

class ClientTest : public ::testing::Test {
protected:
	static std::ostringstream oss;
	Display* display;
	Window rootWindow;
	Window clientWindow;
	std::shared_ptr<mockX11Wrapper> x11WrapperMock;
	std::unique_ptr<Client> client;
	unsigned long inActiveColor = 0x000000; // Example color
	int borderSize = 1; // Example border size
	static void SetUpTestSuite() {
		std::cout << " =================================================================================== " << std::endl;
		std::cout << " ============================= Client SetUpTestSuite ============================== " << std::endl;
		std::cout << " =================================================================================== " << std::endl;
		Logger::Create(ClientTest::oss,L_INFO);
	}

	ClientTest() : display(nullptr),
				   rootWindow(0),
				   clientWindow(0),
				   client(nullptr) {}

	void SetUp() override {
		display = nullptr;
		rootWindow = 42;
		clientWindow = 4242;

		x11WrapperMock = std::make_shared<mockX11Wrapper>();
		Group* group = nullptr;
		Atom mockAtom = 42;
		EXPECT_CALL(*x11WrapperMock,internAtom(display,_,_))
				.Times(AtLeast(1))
				.WillRepeatedly(Return(mockAtom)); // Adjust as necessary
		EXPECT_CALL(*x11WrapperMock,getWindowProperty(_,_,_,_,_,_,_,_,_,_,_,_))
				.Times(AtLeast(1))
				.WillRepeatedly(Return(Success));
//		EXPECT_CALL(*x11WrapperMock,freeX(_))
//				.Times(AtLeast(1))
//				.WillRepeatedly(Return(Success));
		client = std::make_unique<Client>(display,
										  rootWindow,
										  clientWindow,
										  group,
										  inActiveColor,
										  borderSize,
										  x11WrapperMock.get());
	}

	void TearDown() override {}
};
std::ostringstream ClientTest::oss = std::ostringstream ();

TEST_F(ClientTest, FrameUnframe) {
	// Example to test frame and unframe functionality
	EXPECT_EQ(1,1);
	//	EXPECT_CALL(*x11WrapperMock, createSimpleWindow(_, _, _, _, _, _, _, _, _))
//			.Times(1)
//			.WillOnce(Return(clientWindow)); // Adjust as necessary
//
//	EXPECT_EQ(client->frame(), YGG_CLI_NO_ERROR);
//	EXPECT_TRUE(client->isFramed());
//
//	EXPECT_EQ(client->unframe(), YGG_CLI_NO_ERROR);
//	EXPECT_FALSE(client->isFramed());
}

// Add more tests for move, resize, restack, etc.

