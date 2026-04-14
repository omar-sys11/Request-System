#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "user.h"

TEST(UserTest, ConstructorSetsNameCorrectly) {
    User u("id123", "Alice");
    EXPECT_EQ(u.getDisplayName(), "Alice");
}

TEST(UserTest, DefaultConnectionIsTrue) {
    User u("id123", "Alice");
    EXPECT_TRUE(u.isConnected());
}

TEST(UserTest, SetConnectionStatusWorks) {
    User u("id123", "Alice");
    u.setConnectionStatus(false);
    EXPECT_FALSE(u.isConnected());
}

// An interface your real code depends on
class IRequestSender {
public:
    virtual ~IRequestSender() = default;
    virtual bool sendRequest(const std::string& title,
                             const std::string& category) = 0;
};

// The mock — GMock generates fake method implementations
class MockRequestSender : public IRequestSender {
public:
    MOCK_METHOD(bool, sendRequest,
                (const std::string& title, const std::string& category),
                (override));
};

TEST(RequestTest, SendIsCalledOnSubmit) {
    MockRequestSender mockSender;

    // Set an expectation: sendRequest must be called exactly once
    EXPECT_CALL(mockSender, sendRequest("Help me", "Study Help"))
        .Times(1)
        .WillOnce(testing::Return(true));

    // Call whatever code would trigger sendRequest internally
    bool result = mockSender.sendRequest("Help me", "Study Help");

    EXPECT_TRUE(result);
}
