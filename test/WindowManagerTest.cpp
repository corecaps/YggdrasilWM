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
 * @file WindowManagerTest.cpp
 * @brief Unit tests for WindowManager class.
 * @date 2024-02-06
 *
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindowManager.hpp"
#include "Logger.hpp"
#include <cstdlib>
#include <unistd.h>
#include "ConfigHandler.hpp"

void runXephyr () {
	pid_t pid = fork();

	if (pid == -1) {
		// Error handling if fork fails
		perror("Fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		// Child process
		freopen("/dev/null", "w", stdout);
		freopen("/dev/null", "w", stderr);

		execlp("/usr/bin/Xephyr", "/usr/bin/Xephyr", "-ac", "-br", "-noreset", "-screen", "800x600", ":1", nullptr);

		// If exec fails
		perror("Exec failed");
		exit(EXIT_FAILURE);
	} else {
		std::cout << "[       OK ] Xephyr Launched on Display :1" << std::endl;
	}
}

void runXeyes() {
	pid_t pid = fork();

	if (pid == -1) {
		// Error handling if fork fails
		perror("Fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		// Child process
		if (setenv("DISPLAY", ":1", 1) != 0) {
			perror("setenv failed");
			exit(EXIT_FAILURE);
		}

		execlp("/usr/bin/xeyes", "/usr/bin/xeyes", nullptr);

		// If exec fails
		perror("Exec failed");
		exit(EXIT_FAILURE);
	} else {
		std::cout << "[       OK ] Xeyes Launched on Display :1" << std::endl;
	}
}

void killXeyes() {
	system("pkill xeyes");
}

void killXephyr() {
	system("pkill Xephyr");
}

// Mock Logger class for testing
class MockLogger : public Logger {
public:
	MOCK_METHOD(void, Log, (const std::string& message, LogLevel logLevel), (const));
};

class WindowManagerTest : public ::testing::Test {
protected:
	static void SetUpTestSuite() {
		// Setup
		std::cout << " =================================================================================== " << std::endl;
		std::cout << " ======================== WindowManagerTest SetUpTestSuite ========================= " << std::endl;
		std::cout << " =================================================================================== " << std::endl;
		runXephyr();
		sleep(3);
		runXeyes();
		sleep(1);
		MockLogger::Create(std::cout, L_INFO);
	}

	void SetUp() override {
	}
	static void TearDownTestSuite() {
		// Cleanup
		killXeyes();
		killXephyr();
		MockLogger::Destroy();
	}
	void TearDown() override {
	}
};

TEST_F(WindowManagerTest, CreateWithValidDisplay) {
// Try to create a WindowManager with a valid display

WindowManager::Create(":1");

// Check if the WindowManager object is created
ASSERT_TRUE(WindowManager::getInstance());
WindowManager::Destroy();

}

TEST_F(WindowManagerTest, CreateWithInvalidDisplay) {
// Try to create a WindowManager with an invalid display
	// Try to create a WindowManager with an invalid display and expect it to throw
	EXPECT_THROW(WindowManager::Create("invalid_display"), std::runtime_error);
	// Now, check if getting the instance throws, indicating no instance was created
	EXPECT_THROW(WindowManager::getInstance(), std::runtime_error);
	WindowManager::Destroy();
}

TEST_F(WindowManagerTest, initWM) {
	WindowManager::Create(":1");
	WindowManager::getInstance()->Init();
	ASSERT_EQ(WindowManager::getInstance()->getRunning(), true);
	ASSERT_GE(WindowManager::getInstance()->getClients().size(), 0);
	std::cout << "Client size : " << WindowManager::getInstance()->getClients().size() << std::endl;
	ASSERT_NE(WindowManager::getInstance()->getDisplay(), nullptr);
	ASSERT_NE(WindowManager::getInstance()->getRoot(), 0);
	WindowManager::Destroy();
}
