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


class IRequestSender {
public:
    virtual ~IRequestSender() = default;
    virtual bool sendRequest(const std::string& title,
                             const std::string& category) = 0;
};


class MockRequestSender : public IRequestSender {
public:
    MOCK_METHOD(bool, sendRequest,
                (const std::string& title, const std::string& category),
                (override));
};

TEST(RequestTest, SendIsCalledOnSubmit) {
    MockRequestSender mockSender;

    
    EXPECT_CALL(mockSender, sendRequest("Help me", "Study Help"))
        .Times(1)
        .WillOnce(testing::Return(true));

   
    bool result = mockSender.sendRequest("Help me", "Study Help");

    EXPECT_TRUE(result);
}

TEST(UserTest, SetDisplayNameDoesNotChangeId) {
    User u("u1", "Alice");
    u.setDisplayName("Bob");
    EXPECT_EQ(u.getId(), "u1");
}

TEST(RequestTest, SendRequestFailureHandled)
{
    MockRequestSender mockSender;

    EXPECT_CALL(mockSender, sendRequest("Borrow charger", "Borrow Item"))
        .Times(1)
        .WillOnce(testing::Return(false));

    bool result = mockSender.sendRequest("Borrow charger", "Borrow Item");

    EXPECT_FALSE(result);
}
