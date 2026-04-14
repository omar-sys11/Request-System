#include "dashboardwindow.h"
#include "createrequestwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>

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

    QFrame *request1 = new QFrame(this);
    request1->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *request1Layout = new QVBoxLayout(request1);
    request1Layout->addWidget(new QLabel("Need calculus notes", this));
    request1Layout->addWidget(new QLabel("Category: Study Help", this));
    request1Layout->addWidget(new QLabel("Location: HUSS Building", this));
    request1Layout->addWidget(new QLabel("Status: Open", this));

    QFrame *request2 = new QFrame(this);
    request2->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *request2Layout = new QVBoxLayout(request2);
    request2Layout->addWidget(new QLabel("Looking for a study partner", this));
    request2Layout->addWidget(new QLabel("Category: Study Partner", this));
    request2Layout->addWidget(new QLabel("Location: Library", this));
    request2Layout->addWidget(new QLabel("Status: Open", this));

    QFrame *request3 = new QFrame(this);
    request3->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *request3Layout = new QVBoxLayout(request3);
    request3Layout->addWidget(new QLabel("Need a charger urgently", this));
    request3Layout->addWidget(new QLabel("Category: Borrow Item", this));
    request3Layout->addWidget(new QLabel("Location: Dorm 5", this));
    request3Layout->addWidget(new QLabel("Status: Open", this));

    requestsLayout->addWidget(request1);
    requestsLayout->addWidget(request2);
    requestsLayout->addWidget(request3);
    requestsLayout->addStretch();

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

                    QFrame *newRequest = new QFrame(this);
                    newRequest->setFrameShape(QFrame::StyledPanel);

                    QVBoxLayout *layout = new QVBoxLayout(newRequest);
                    layout->addWidget(new QLabel(title, this));
                    layout->addWidget(new QLabel("Category: " + category, this));
                    layout->addWidget(new QLabel("Location: " + location, this));
                    layout->addWidget(new QLabel("Status: Open", this));

                    requestsLayout->insertWidget(0, newRequest);
                });

        createRequestWindow->show();
    });
}
