#include "dashboardwindow.h"
#include "createrequestwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <Qt>

DashboardWindow::DashboardWindow(const User &user, NetworkClient *client, QWidget *parent)
    : QWidget(parent), currentUser(user), networkClient(client) {

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

                    refreshRequests();
			
		    networkClient->sendNewRequest(
                        title, category, location,
                        QString::fromStdString(currentUser.getDisplayName())
		    );
                });


        win->show();
    });
    titleText->setStyleSheet("font-weight: bold; font-size: 14px;");

    cardLayout->addWidget(titleText);
    cardLayout->addWidget(categoryText);
    cardLayout->addWidget(locationText);
    cardLayout->addWidget(statusText);

    requestsLayout->insertWidget(0, requestCard, 0, Qt::AlignHCenter);
}

void DashboardWindow::onMessageReceived(QString type, QString requestId, QString title,
                                         QString category, QString location,
                                         QString poster, QString status) {
    if (type == "new_request") {
        requestManager.addRequest(title, category, location, poster);
        refreshRequests();
    }
    else if (type == "accept_request" || type == "close_request") {
        requestManager.acceptRequest(requestId, poster);
        refreshRequests();
    }
}

void DashboardWindow::onDisconnected() {
    QMessageBox::warning(this, "Disconnected", "Lost connection to server.");
}

void DashboardWindow::refreshRequests() {
    QLayoutItem *item;
    while ((item = requestsLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    for (const auto &r : requestManager.getRequests()) {

        QFrame *card = new QFrame(this);
        card->setFrameShape(QFrame::StyledPanel);

        QVBoxLayout *layout = new QVBoxLayout(card);

        QLabel *title = new QLabel(r.title, this);
        QLabel *cat = new QLabel("Category: " + r.category, this);
        QLabel *loc = new QLabel("Location: " + r.location, this);

        QString statusText =
            (r.status == RequestStatus::Open) ? "Open" :
            (r.status == RequestStatus::Accepted) ? "Accepted" :
                                                    "Closed";

        QLabel *status = new QLabel("Status: " + statusText, this);

        QPushButton *acceptBtn = new QPushButton("Accept", this);
        QPushButton *closeBtn = new QPushButton("Close", this);

        connect(acceptBtn, &QPushButton::clicked, this, [=]() {
            if (!requestManager.acceptRequest(r.id, QString::fromStdString(currentUser.getId()))) {
                QMessageBox::warning(this, "Error", "Cannot accept request.");
                return;
            }
	    networkClient->sendAccept(r.id, QString::fromStdString(currentUser.getDisplayName()));

            refreshRequests();
        });

        connect(closeBtn, &QPushButton::clicked, this, [=]() {
            if (!requestManager.closeRequest(r.id, QString::fromStdString(currentUser.getId()))) {
                QMessageBox::warning(this, "Error", "Cannot close request.");
                return;
            }
            networkClient->sendClose(r.id);

            refreshRequests();
        });

        layout->addWidget(title);
        layout->addWidget(cat);
        layout->addWidget(loc);
        layout->addWidget(status);
        layout->addWidget(acceptBtn);
        layout->addWidget(closeBtn);

        requestsLayout->addWidget(card);
    }

    requestsLayout->addStretch();
}
void DashboardWindow::addRequestCard(QString title, QString category, QString location)
{
    displayRequestCard(title, category, location, "Open");
}
