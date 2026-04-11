#include "FeedWindow.h"

#include <QDateTime>
#include <QFrame>
#include <QLabel>

// your items
#include "MessageItem.h"
#include "RequestItem.h"
#include "AcceptItem.h"

FeedWindow::FeedWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Feed System");
    resize(500, 700);

    auto* mainLayout = new QVBoxLayout(this);

    // ---------------- INPUT ----------------
    typeBox = new QComboBox();
    typeBox->addItem("Message");
    typeBox->addItem("Request");
    typeBox->addItem("Accept");

    senderInput = new QLineEdit();
    senderInput->setPlaceholderText("Sender");

    contentInput = new QLineEdit();
    contentInput->setPlaceholderText("Content");

    addButton = new QPushButton("Add");

    auto* inputLayout = new QHBoxLayout();
    inputLayout->addWidget(typeBox);
    inputLayout->addWidget(senderInput);
    inputLayout->addWidget(contentInput);
    inputLayout->addWidget(addButton);

    mainLayout->addLayout(inputLayout);

    // ---------------- FEED AREA ----------------
    feedContainer = new QWidget();
    feedLayout = new QVBoxLayout(feedContainer);
    feedLayout->setAlignment(Qt::AlignTop);

    scrollArea = new QScrollArea();
    scrollArea->setWidget(feedContainer);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea);

    // ---------------- SIGNAL ----------------
    connect(addButton, &QPushButton::clicked,
            this, &FeedWindow::onAddClicked);
}

void FeedWindow::onAddClicked()
{
    std::string sender = senderInput->text().toStdString();
    std::string content = contentInput->text().toStdString();

    long long timestamp = QDateTime::currentSecsSinceEpoch();

    std::unique_ptr<FeedItem> item;

    QString type = typeBox->currentText();

    if (type == "Message")
    {
        item = std::make_unique<MessageItem>(sender, content, timestamp);
    }
    else if (type == "Request")
    {
        item = std::make_unique<RequestItem>(sender, content, timestamp);
    }
    else if (type == "Accept")
    {
        item = std::make_unique<AcceptItem>(sender, content, timestamp);
    }

    if (item)
    {
        board.add(std::move(item));
        refreshFeed();
    }

    senderInput->clear();
    contentInput->clear();
}
