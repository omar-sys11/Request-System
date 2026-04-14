#include "dashboardwindow.h"
#include "createrequestwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <QUuid>

DashboardWindow::DashboardWindow(const User &user, QWidget *parent)
    : QWidget(parent), currentUser(user) {

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
        CreateRequestWindow *win = new CreateRequestWindow();

        connect(win, &CreateRequestWindow::requestCreated,
                this, [this](QString title, QString category, QString location) {

                    requestManager.addRequest(
                        title,
                        category,
                        location,
                        QString::fromStdString(currentUser.getId())
                    );

                    refreshRequests();
                });

        win->show();
    });

    refreshRequests();
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
            refreshRequests();
        });

        connect(closeBtn, &QPushButton::clicked, this, [=]() {
            if (!requestManager.closeRequest(r.id, QString::fromStdString(currentUser.getId()))) {
                QMessageBox::warning(this, "Error", "Cannot close request.");
                return;
            }
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