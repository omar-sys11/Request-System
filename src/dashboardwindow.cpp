#include "dashboardwindow.h"
#include "createrequestwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <QDateTime>
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

    setStyleSheet(
        "QWidget {"
        "   background-color: #f5f5f5;"
        "   color: #000000;"
        "}"
        "QLabel {"
        "   color: #000000;"
        "   background-color: transparent;"
        "}"
        "QPushButton {"
        "   color: #000000;"
        "   background-color: #e0e0e0;"
        "   border: 1px solid #888888;"
        "   border-radius: 4px;"
        "   padding: 6px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d0d0d0;"
        "}"
    );

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
    QString requestId = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString userId = QString::fromStdString(currentUser.getId());
    QString poster = QString::fromStdString(currentUser.getDisplayName());

    requestManager.addRequest(title, category, location, userId, requestId);

    if (networkClient != nullptr && networkClient->isConnected()) {
        networkClient->sendNewRequest(requestId, title, category, location, poster);
    }

    refreshRequests();
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

    QString currentUserId = QString::fromStdString(currentUser.getId());
    QString currentUserName = QString::fromStdString(currentUser.getDisplayName());

    for (const auto &request : requestManager.getRequests()) {
        QString statusText;

        if (request.status == RequestStatus::Open) {
            statusText = "Open";
        } else if (request.status == RequestStatus::Accepted) {
            statusText = "Accepted";
        } else {
            statusText = "Closed";
        }

        QFrame *requestCard = new QFrame(this);
        requestCard->setObjectName("requestCard");
        requestCard->setFrameShape(QFrame::NoFrame);
        requestCard->setFixedSize(420, 170);

        requestCard->setStyleSheet(
            "#requestCard {"
            "   border: 1px solid #bfbfbf;"
            "   border-radius: 8px;"
            "   background-color: #ffffff;"
            "}"
        );

        QVBoxLayout *cardLayout = new QVBoxLayout(requestCard);
        cardLayout->setContentsMargins(12, 10, 12, 10);
        cardLayout->setSpacing(6);

        QLabel *titleText = new QLabel(request.title, requestCard);
        QLabel *categoryText = new QLabel("Category: " + request.category, requestCard);
        QLabel *locationText = new QLabel("Location: " + request.location, requestCard);
        QLabel *statusLabel = new QLabel("Status: " + statusText, requestCard);

        QString normalLabelStyle =
            "QLabel {"
            "   color: #000000;"
            "   background-color: transparent;"
            "   border: none;"
            "   font-size: 13px;"
            "}";

        QString titleLabelStyle =
            "QLabel {"
            "   color: #000000;"
            "   background-color: transparent;"
            "   border: none;"
            "   font-weight: bold;"
            "   font-size: 14px;"
            "}";

        titleText->setStyleSheet(titleLabelStyle);
        categoryText->setStyleSheet(normalLabelStyle);
        locationText->setStyleSheet(normalLabelStyle);
        statusLabel->setStyleSheet(normalLabelStyle);

        cardLayout->addWidget(titleText);
        cardLayout->addWidget(categoryText);
        cardLayout->addWidget(locationText);
        cardLayout->addWidget(statusLabel);

        QHBoxLayout *buttonLayout = new QHBoxLayout;

        bool isOwnRequest = (request.ownerId == currentUserId);

        if (request.status == RequestStatus::Open && isOwnRequest) {
            QPushButton *closeButton = new QPushButton("Close", requestCard);

            connect(closeButton, &QPushButton::clicked, this, [this, request, currentUserId]() {
                if (!requestManager.closeRequest(request.id, currentUserId)) {
                    QMessageBox::warning(this, "Error", "Cannot close this request.");
                    return;
                }

                if (networkClient != nullptr && networkClient->isConnected()) {
                    networkClient->sendClose(request.id);
                }

                refreshRequests();
            });

            buttonLayout->addWidget(closeButton);
        }

        if (request.status == RequestStatus::Open && !isOwnRequest) {
            QPushButton *acceptButton = new QPushButton("Accept", requestCard);

            connect(acceptButton, &QPushButton::clicked, this, [this, request, currentUserId, currentUserName]() {
                if (!requestManager.acceptRequest(request.id, currentUserId)) {
                    QMessageBox::warning(this, "Error", "Cannot accept this request.");
                    return;
                }

                if (networkClient != nullptr && networkClient->isConnected()) {
                    networkClient->sendAccept(request.id, currentUserName);
                }

                refreshRequests();
            });

            buttonLayout->addWidget(acceptButton);
        }

        cardLayout->addLayout(buttonLayout);

        requestsLayout->insertWidget(0, requestCard, 0, Qt::AlignHCenter);
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
    Q_UNUSED(status);

    if (type == "new_request") {
        requestManager.addRequest(title, category, location, poster, requestId);
        refreshRequests();
    } else if (type == "accept_request") {
        requestManager.markAccepted(requestId);
        refreshRequests();
    } else if (type == "close_request") {
        requestManager.markClosed(requestId);
        refreshRequests();
    }
}

void DashboardWindow::onDisconnected()
{
    QMessageBox::warning(this, "Disconnected", "Lost connection to server.");
}