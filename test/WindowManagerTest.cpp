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
 * @date 2024-02-02
 *
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "window_manager.hpp"
#include "Logger.hpp"
#include <cstdlib>
#include <unistd.h>

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
		std::cout << "Xephyr Launched on Display :1" << std::endl;
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
		std::cout << "Xeyes Launched on Display :1" << std::endl;
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
	explicit MockLogger(std::ostream& output = std::cout, LogLevel logLevel = L_INFO)
			: Logger(output, logLevel) {}
	MOCK_METHOD(void, Log, (const std::string& message, LogLevel logLevel), (const));
};

class WindowManagerTest : public ::testing::Test {
protected:
	static void SetUpTestSuite() {
		// Setup
		std::cout << " =================================================================================== " << std::endl;
		std::cout << " ============================== WindowManagerTest SetUpTestSuite ============================== " << std::endl;
		std::cout << " =================================================================================== " << std::endl;
		runXephyr();
		sleep(3);
		runXeyes();
		sleep(1);
	}

	void SetUp() override {
		mockLogger = std::make_shared<MockLogger>();
	}
	static void TearDownTestSuite() {
		// Cleanup
		killXeyes();
		killXephyr();
	}
	void TearDown() override {
	mockLogger.reset();
	}

	std::shared_ptr<MockLogger> mockLogger;
};

TEST_F(WindowManagerTest, CreateWithValidDisplay) {
// Expect the Log method to be called with INFO level
EXPECT_CALL(*mockLogger, Log(::testing::_, L_INFO))
.Times(::testing::AtLeast(1));

// Try to create a WindowManager with a valid display
auto windowManager = WindowManager::Create(*mockLogger,":1");

// Check if the WindowManager object is created
ASSERT_TRUE(windowManager);

// Check if the Logger instance is correctly set
ASSERT_EQ(&windowManager->getLogger(), mockLogger.get());
}

TEST_F(WindowManagerTest, CreateWithInvalidDisplay) {
// Expect the Log method to be called with ERROR level
EXPECT_CALL(*mockLogger, Log(::testing::_, L_ERROR))
.Times(::testing::AtLeast(1));

// Try to create a WindowManager with an invalid display
auto windowManager = WindowManager::Create(*mockLogger, "invalid_display");

// Check if the WindowManager object is not created
ASSERT_FALSE(windowManager);
}

TEST_F(WindowManagerTest, GetLogger) {
	EXPECT_CALL(*mockLogger, Log(::testing::_, L_INFO))
			.Times(::testing::AtLeast(1));
auto windowManager = WindowManager::Create(*mockLogger,":1");
ASSERT_EQ(&windowManager->getLogger(), mockLogger.get());
}
TEST_F(WindowManagerTest, initWM) {
	EXPECT_CALL(*mockLogger, Log(::testing::_, L_INFO))
			.Times(::testing::AtLeast(1));
	auto windowManager = WindowManager::Create(*mockLogger,":1");
	ASSERT_EQ(&windowManager->getLogger(), mockLogger.get());
	windowManager->Init();
	ASSERT_EQ(windowManager->getRunning(), true);
	ASSERT_GE(windowManager->getClients().size(), 0);
	std::cout << "Client size : " << windowManager->getClients().size() << std::endl;
	ASSERT_NE(windowManager->getLayoutManager(), nullptr);
	ASSERT_NE(windowManager->getDisplay(), nullptr);
	ASSERT_NE(windowManager->getRoot(), 0);
}
