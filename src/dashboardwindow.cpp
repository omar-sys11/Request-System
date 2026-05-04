#include "dashboardwindow.h"
#include "createrequestwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <Qt>

DashboardWindow::DashboardWindow(const User &user, NetworkClient *client, QWidget *parent)
    : QWidget(parent), currentUser(user), networkClient(client)
{
    titleLabel = new QLabel(
        QString("Welcome, %1 — Live Requests Feed")
            .arg(QString::fromStdString(currentUser.getDisplayName())),
        this
    );

    newRequestButton = new QPushButton("New Request", this);

    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(newRequestButton);

    requestsLayout = new QVBoxLayout;
    requestsLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    requestsLayout->setSpacing(10);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(requestsLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setWindowTitle("Dashboard");
    resize(500, 400);

    connect(networkClient, &NetworkClient::messageReceived,
            this, &DashboardWindow::onMessageReceived);

    connect(networkClient, &NetworkClient::disconnectedFromServer,
            this, &DashboardWindow::onDisconnected);

    connect(newRequestButton, &QPushButton::clicked, this, [this]() {
        CreateRequestWindow *createRequestWindow = new CreateRequestWindow();

        connect(createRequestWindow, &CreateRequestWindow::requestSubmitted,
                this, &DashboardWindow::addRequestCard);

        createRequestWindow->show();
    });
}

void DashboardWindow::addRequestCard(QString title, QString category, QString location)
{
    QString userId = QString::fromStdString(currentUser.getId());
    QString poster = QString::fromStdString(currentUser.getDisplayName());

    requestManager.addRequest(title, category, location, userId);

    if (networkClient != nullptr && networkClient->isConnected()) {
        networkClient->sendNewRequest(title, category, location, poster);
    }

    refreshRequests();
}

void DashboardWindow::displayRequestCard(QString title, QString category, QString location, QString status)
{
    QFrame *requestCard = new QFrame(this);
    requestCard->setFrameShape(QFrame::StyledPanel);
    requestCard->setFixedSize(420, 130);

    requestCard->setStyleSheet(
        "QFrame {"
        "   border: 1px solid #bfbfbf;"
        "   border-radius: 8px;"
        "   background-color: #ffffff;"
        "}"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(requestCard);
    cardLayout->setContentsMargins(12, 10, 12, 10);
    cardLayout->setSpacing(6);

    QLabel *titleText = new QLabel(title, requestCard);
    QLabel *categoryText = new QLabel("Category: " + category, requestCard);
    QLabel *locationText = new QLabel("Location: " + location, requestCard);
    QLabel *statusText = new QLabel("Status: " + status, requestCard);

    titleText->setStyleSheet("font-weight: bold; font-size: 14px;");

    cardLayout->addWidget(titleText);
    cardLayout->addWidget(categoryText);
    cardLayout->addWidget(locationText);
    cardLayout->addWidget(statusText);

    requestsLayout->insertWidget(0, requestCard, 0, Qt::AlignHCenter);
}

void DashboardWindow::refreshRequests()
{
    QLayoutItem *item;

    while ((item = requestsLayout->takeAt(0)) != nullptr) {
        if (item->widget() != nullptr) {
            delete item->widget();
        }

        delete item;
    }

    for (const auto &request : requestManager.getRequests()) {
        QString statusText;

        if (request.status == RequestStatus::Open) {
            statusText = "Open";
        } else if (request.status == RequestStatus::Accepted) {
            statusText = "Accepted";
        } else {
            statusText = "Closed";
        }

        displayRequestCard(request.title, request.category, request.location, statusText);
    }
}

void DashboardWindow::onMessageReceived(QString type,
                                        QString requestId,
                                        QString title,
                                        QString category,
                                        QString location,
                                        QString poster,
                                        QString status)
{
    Q_UNUSED(requestId);
    Q_UNUSED(status);

    if (type == "new_request") {
        requestManager.addRequest(title, category, location, poster);
        refreshRequests();
    }
}

void DashboardWindow::onDisconnected()
{
    QMessageBox::warning(this, "Disconnected", "Lost connection to server.");
}