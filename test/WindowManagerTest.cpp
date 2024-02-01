//
// Created by corecaps on 1/10/24.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "window_manager.hpp"
#include "Logger.hpp"
#include <cstdlib>


// Mock Logger class for testing
class MockLogger : public Logger {
public:
	explicit MockLogger(std::ostream& output = std::cout, LogLevel logLevel = L_INFO)
			: Logger(output, logLevel) {}
	MOCK_METHOD(void, Log, (const std::string& message, LogLevel logLevel), (const));
};

class WindowManagerTest : public ::testing::Test {
protected:
	void SetUp() override {
		// Create a mock logger
		mockLogger = std::make_shared<MockLogger>();
	}

	void TearDown() override {
		// Cleanup
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

// Add more tests for other functions and behaviors as needed

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}