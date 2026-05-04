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

TEST(UserTest, SetDisplayNameDoesNotChangeId) {
    User u("u1", "Alice");
    u.setDisplayName("Bob");
    EXPECT_EQ(u.getId(), "u1");
}


TEST(UserTest, SetDisplayNameWorks) {
    User u("id123", "Alice");

    u.setDisplayName("Charlie");

    EXPECT_EQ(u.getDisplayName(), "Charlie");
}

TEST(UserTest, ConnectionStatusCanBeSetBackToTrue) {
    User u("id123", "Alice");

    u.setConnectionStatus(false);
    u.setConnectionStatus(true);

    EXPECT_TRUE(u.isConnected());
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

class RequestSubmitter {
public:
    explicit RequestSubmitter(IRequestSender* sender)
        : sender(sender) {}

    bool submit(const std::string& title, const std::string& category) {
        if (title.empty() || category.empty()) {
            return false;
        }

        return sender->sendRequest(title, category);
    }

private:
    IRequestSender* sender;
};

TEST(RequestSubmitterTest, SendsValidRequest) {
    MockRequestSender mockSender;
    RequestSubmitter submitter(&mockSender);

    EXPECT_CALL(mockSender, sendRequest("Help me", "Study Help"))
        .Times(1)
        .WillOnce(testing::Return(true));

    EXPECT_TRUE(submitter.submit("Help me", "Study Help"));
}

TEST(RequestSubmitterTest, ReturnsFalseWhenSenderFails) {
    MockRequestSender mockSender;
    RequestSubmitter submitter(&mockSender);

    EXPECT_CALL(mockSender, sendRequest("Borrow charger", "Borrow Item"))
        .Times(1)
        .WillOnce(testing::Return(false));

    EXPECT_FALSE(submitter.submit("Borrow charger", "Borrow Item"));
}

TEST(RequestSubmitterTest, DoesNotSendEmptyTitle) {
    MockRequestSender mockSender;
    RequestSubmitter submitter(&mockSender);

    EXPECT_CALL(mockSender, sendRequest(testing::_, testing::_))
        .Times(0);

    EXPECT_FALSE(submitter.submit("", "Study Help"));
}

TEST(RequestSubmitterTest, DoesNotSendEmptyCategory) {
    MockRequestSender mockSender;
    RequestSubmitter submitter(&mockSender);

    EXPECT_CALL(mockSender, sendRequest(testing::_, testing::_))
        .Times(0);

    EXPECT_FALSE(submitter.submit("Help me", ""));
}
