#pragma once
#include <string>

enum class RequestStatus {
    Open,
    Accepted,
    Closed
};

enum class Category {
    BorrowItem,
    QuickQuestion,
    TechHelp,
    StudyPartner,
    Other
};

enum class Urgency {
    Low,
    Medium,
    High
};
