#include "dashboardwindow.h"
#include "createrequestwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <Qt>

DashboardWindow::DashboardWindow(const User &user, QWidget *parent)
    : QWidget(parent), currentUser(user)
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

    titleText->setStyleSheet("font-weight: bold; font-size: 14px;");

    cardLayout->addWidget(titleText);
    cardLayout->addWidget(categoryText);
    cardLayout->addWidget(locationText);
    cardLayout->addWidget(statusText);

    requestsLayout->insertWidget(0, requestCard, 0, Qt::AlignHCenter);
}

void DashboardWindow::addRequestCard(QString title, QString category, QString location)
{
    displayRequestCard(title, category, location, "Open");
}