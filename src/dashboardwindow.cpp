#include "dashboardwindow.h"
#include "createrequestwindow.h"
#include "RequestService.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>

static QString statusToString(RequestStatus status)
{
    switch (status) {
        case RequestStatus::Open: return "Open";
        case RequestStatus::Accepted: return "Accepted";
        case RequestStatus::Closed: return "Closed";
    }
    return "Unknown";
}

DashboardWindow::DashboardWindow(const User& user, QWidget *parent)
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

        connect(createRequestWindow, &CreateRequestWindow::requestCreated,
                this, [this](QString title, QString category, QString location) {

            service.addRequest(title, category, location);

            int index = (int)service.getRequests().size() - 1;
            Request &req = service.getRequests()[index];

            QFrame *frame = new QFrame(this);
            frame->setFrameShape(QFrame::StyledPanel);

            QVBoxLayout *layout = new QVBoxLayout(frame);

            QLabel *statusLabel = new QLabel("Status: Open", this);

            QPushButton *acceptBtn = new QPushButton("Accept", this);
            QPushButton *closeBtn = new QPushButton("Close", this);

            layout->addWidget(new QLabel(req.title, this));
            layout->addWidget(new QLabel("Category: " + req.category, this));
            layout->addWidget(new QLabel("Location: " + req.location, this));
            layout->addWidget(statusLabel);
            layout->addWidget(acceptBtn);
            layout->addWidget(closeBtn);

            connect(acceptBtn, &QPushButton::clicked, this, [=]() mutable {
                service.acceptRequest(index);
                statusLabel->setText("Status: Accepted");
            });

            connect(closeBtn, &QPushButton::clicked, this, [=]() mutable {
                service.closeRequest(index);
                statusLabel->setText("Status: Closed");
            });

            requestsLayout->insertWidget(0, frame);
        });

        createRequestWindow->show();
    });
}
