#include "loginwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>
#include <QUuid>
#include "dashboardwindow.h"
#include "user.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent) {
    titleLabel = new QLabel("Request System Login", this);
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    nameLabel = new QLabel("Display Name:", this);
    ipLabel = new QLabel("Server IP:", this);
    portLabel = new QLabel("Port:", this);
    statusLabel = new QLabel("Status: Disconnected", this);

    nameEdit = new QLineEdit(this);
    ipEdit = new QLineEdit(this);
    portEdit = new QLineEdit(this);

    connectButton = new QPushButton("Connect", this);

    ipEdit->setText("127.0.0.1");
    portEdit->setText("12345");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(nameLabel, nameEdit);
    formLayout->addRow(ipLabel, ipEdit);
    formLayout->addRow(portLabel, portEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(connectButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setWindowTitle("Login");
    resize(350, 200);

    connect(connectButton, &QPushButton::clicked,
            this, &LoginWindow::onConnectButtonClicked);
}

void LoginWindow::onConnectButtonClicked() {
    QString name = nameEdit->text().trimmed();
    QString ip = ipEdit->text().trimmed();
    QString portText = portEdit->text().trimmed();

    if (name.isEmpty() || ip.isEmpty() || portText.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Please fill in all fields.");
        return;
    }

    bool ok;
    quint16 port = portText.toUShort(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Invalid Port",
                             "Please enter a valid port number.");
        return;
    }

    // SIMPLE SERVICE CALL
    UserService service;
    User currentUser = service.createUser(name.toStdString());

    statusLabel->setText("Status: Connected");

    DashboardWindow *dashboard = new DashboardWindow(currentUser);
    dashboard->show();

    this->close();

    emit connectRequested(name, ip, port);
}
