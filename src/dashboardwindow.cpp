#include "dashboardwindow.h"
#include "createrequestwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>

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

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(requestsLayout);

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

    QVBoxLayout *cardLayout = new QVBoxLayout(requestCard);
    cardLayout->addWidget(new QLabel(title, requestCard));
    cardLayout->addWidget(new QLabel("Category: " + category, requestCard));
    cardLayout->addWidget(new QLabel("Location: " + location, requestCard));
    cardLayout->addWidget(new QLabel("Status: " + status, requestCard));

    requestsLayout->insertWidget(0, requestCard);
}

void DashboardWindow::addRequestCard(QString title, QString category, QString location)
{
    displayRequestCard(title, category, location, "Open");
}