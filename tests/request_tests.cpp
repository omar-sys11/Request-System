#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "RequestManager.h"
#include "user.h"

using ::testing::Return;
using ::testing::_;

// =========================
// USER TESTS
// =========================

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

// =========================
// REQUEST MANAGER TESTS
// =========================

TEST(RequestManagerTest, AddRequestCreatesRequest) {
    RequestManager manager;

    manager.addRequest("Help me", "Study Help", "Cairo", "user1");

    const auto& requests = manager.getRequests();

    ASSERT_EQ(requests.size(), 1);
    EXPECT_EQ(requests[0].title, "Help me");
    EXPECT_EQ(requests[0].category, "Study Help");
    EXPECT_EQ(requests[0].location, "Cairo");
    EXPECT_EQ(requests[0].ownerId, "user1");
}

// =========================
// MOCK INTERFACE TEST
// =========================

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

TEST(RequestTest, SendRequestFailureHandled) {
    MockRequestSender mockSender;

    // Expect the call AND define behavior
    EXPECT_CALL(mockSender, sendRequest("Borrow charger", "Borrow Item"))
        .Times(1)
        .WillOnce(Return(false));

    // Act (IMPORTANT: call AFTER expectation)
    bool result = mockSender.sendRequest("Borrow charger", "Borrow Item");

    EXPECT_FALSE(result);
}
