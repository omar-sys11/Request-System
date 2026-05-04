#include "dashboardwindow.h"
#include "createrequestwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QMessageBox>

DashboardWindow::DashboardWindow(const User &user, QWidget *parent)
    : QWidget(parent), currentUser(user)
{
    titleLabel = new QLabel("Live Requests Feed", this);
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

    if (!databaseManager.initialize()) {
        QMessageBox::warning(this, "Database Error",
                             "Could not open database:\n" + databaseManager.getLastError());
    }

    loadRequestsFromDatabase();

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

void DashboardWindow::loadRequestsFromDatabase()
{
    QList<StoredRequest> savedRequests = databaseManager.loadRequests();

    if (savedRequests.isEmpty()) {
        displayRequestCard("Need calculus notes", "Study Help", "HUSS Building", "Open");
        displayRequestCard("Looking for a study partner", "Study Partner", "Library", "Open");
        displayRequestCard("Need a charger urgently", "Borrow Item", "Dorm 5", "Open");
        return;
    }

    for (const StoredRequest& request : savedRequests) {
        displayRequestCard(request.title, request.category, request.location, request.status);
    }
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
    QString status = "Open";

    bool saved = databaseManager.addRequest(title, category, location, status);

    if (!saved) {
        QMessageBox::warning(this, "Database Error",
                             "The request could not be saved:\n" + databaseManager.getLastError());
        return;
    }

    displayRequestCard(title, category, location, status);
}
