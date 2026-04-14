#include "dashboardwindow.h"
#include "createrequestwindow.h"
#include "RequestService.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <QUuid>

struct Request {
    QString id;
    QString ownerId;
    QString title;
    QString category;
    QString location;
    QString status; 
};

static std::vector<Request> requests;

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

    auto renderRequest = [this](const Request& r) {

        QFrame *card = new QFrame(this);
        card->setFrameShape(QFrame::StyledPanel);

        QVBoxLayout *layout = new QVBoxLayout(card);

        QLabel *title = new QLabel(r.title, this);
        QLabel *cat = new QLabel("Category: " + r.category, this);
        QLabel *loc = new QLabel("Location: " + r.location, this);
        QLabel *status = new QLabel("Status: " + r.status, this);

        QPushButton *acceptBtn = new QPushButton("Accept", this);
        QPushButton *closeBtn = new QPushButton("Close", this);

        // Accept request
        connect(acceptBtn, &QPushButton::clicked, this, [=]() mutable {

            if (r.ownerId == QString::fromStdString(currentUser.getId())) {
                QMessageBox::warning(this, "Invalid Action",
                                     "You cannot accept your own request.");
                return;
            }

            for (auto &req : requests) {
                if (req.id == r.id && req.status == "Open") {
                    req.status = "Accepted";
                    QMessageBox::information(this, "Accepted",
                                             "You accepted the request.");
                    refresh();
                    return;
                }
            }
        });

        connect(closeBtn, &QPushButton::clicked, this, [=]() mutable {

            if (r.ownerId != QString::fromStdString(currentUser.getId())) {
                QMessageBox::warning(this, "Invalid Action",
                                     "Only the owner can close this request.");
                return;
            }

            for (auto &req : requests) {
                if (req.id == r.id) {
                    req.status = "Closed";
                    refresh();
                    return;
                }
            }
        });

        layout->addWidget(title);
        layout->addWidget(cat);
        layout->addWidget(loc);
        layout->addWidget(status);
        layout->addWidget(acceptBtn);
        layout->addWidget(closeBtn);

        requestsLayout->addWidget(card);
    };

    auto refresh = [this, renderRequest]() {
        QLayoutItem *item;
        while ((item = requestsLayout->takeAt(0))) {
            delete item->widget();
            delete item;
        }

        for (const auto &r : requests) {
            renderRequest(r);
        }

        requestsLayout->addStretch();
    };

    this->setProperty("refresh", QVariant::fromValue((void*)&refresh));

    connect(newRequestButton, &QPushButton::clicked, this, [this, refresh]() {

        CreateRequestWindow *createRequestWindow = new CreateRequestWindow();

        connect(createRequestWindow, &CreateRequestWindow::requestCreated,
                this, [this, refresh](QString title, QString category, QString location) {

            Request r;
            r.id = QUuid::createUuid().toString();
            r.ownerId = QString::fromStdString(currentUser.getId());
            r.title = title;
            r.category = category;
            r.location = location;
            r.status = "Open";

            requests.push_back(r);

            RequestService service;
            service.handleRequest(title, category, location);

            refresh();
        });

        createRequestWindow->show();
    });

    setLayout(mainLayout);
    setWindowTitle("Dashboard");
    resize(500, 400);

    refresh();
}
