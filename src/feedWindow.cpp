#include "FeedWindow.h"

#include <QLabel>
#include <QFrame>
#include <QDateTime>

// your derived classes
#include "RequestItem.h"
#include "MessageItem.h"
#include "AcceptItem.h"

FeedWindow::FeedWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Feed System");
    resize(500, 700);

    auto* mainLayout = new QVBoxLayout(this);

    // INPUT ROW
    typeBox = new QComboBox();
    typeBox->addItem("Message");
    typeBox->addItem("Request");
    typeBox->addItem("Accept");

    senderInput = new QLineEdit();
    senderInput->setPlaceholderText("Sender");

    contentInput = new QLineEdit();
    contentInput->setPlaceholderText("Content");

    auto* addBtn = new QPushButton("Add");

    auto* inputLayout = new QHBoxLayout();
    inputLayout->addWidget(typeBox);
    inputLayout->addWidget(senderInput);
    inputLayout->addWidget(contentInput);
    inputLayout->addWidget(addBtn);

    mainLayout->addLayout(inputLayout);

    // FEED AREA (scrollable)
    feedContainer = new QWidget();
    feedLayout = new QVBoxLayout(feedContainer);
    feedLayout->setAlignment(Qt::AlignTop);

    scrollArea = new QScrollArea();
    scrollArea->setWidget(feedContainer);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea);

    connect(addBtn, &QPushButton::clicked, this, &FeedWindow::onAddClicked);
}
