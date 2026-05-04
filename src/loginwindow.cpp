#include "loginwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>

#include "dashboardwindow.h"
#include "user.h"
#include "UserService.h"
#include "networkclient.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    titleLabel = new QLabel("Request System Login", this);

    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);

    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    nameLabel = new QLabel("Username:", this);
    passwordLabel = new QLabel("Password:", this);
    ipLabel = new QLabel("Server IP:", this);
    portLabel = new QLabel("Port:", this);
    statusLabel = new QLabel("Status: Not logged in", this);

    nameEdit = new QLineEdit(this);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    ipEdit = new QLineEdit(this);
    ipEdit->setText("127.0.0.1");

    portEdit = new QLineEdit(this);
    portEdit->setText("12345");

    loginButton = new QPushButton("Login / Connect", this);
    signUpButton = new QPushButton("Sign Up", this);

    networkClient = new NetworkClient(this);

    connect(networkClient, &NetworkClient::connectedToServer,
            this, &LoginWindow::onConnectedToServer);

    connect(networkClient, &NetworkClient::connectionError,
            this, &LoginWindow::onConnectionError);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(nameLabel, nameEdit);
    formLayout->addRow(passwordLabel, passwordEdit);
    formLayout->addRow(ipLabel, ipEdit);
    formLayout->addRow(portLabel, portEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(signUpButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setWindowTitle("Login");
    resize(380, 280);

    setStyleSheet(
        "QWidget {"
        "   background-color: #f5f5f5;"
        "   color: #000000;"
        "}"
        "QLabel {"
        "   color: #000000;"
        "}"
        "QLineEdit {"
        "   background-color: #ffffff;"
        "   color: #000000;"
        "   border: 1px solid #999999;"
        "   border-radius: 4px;"
        "   padding: 4px;"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #4a90e2;"
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
        "QPushButton:disabled {"
        "   color: #777777;"
        "   background-color: #eeeeee;"
        "}"
    );

    if (!databaseManager.initialize()) {
        QMessageBox::warning(this, "Database Error",
                             "Could not initialize database:\n" + databaseManager.getLastError());
    }

    connect(loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginButtonClicked);

    connect(signUpButton, &QPushButton::clicked,
            this, &LoginWindow::onSignUpButtonClicked);
}

void LoginWindow::onSignUpButtonClicked()
{
    QString username = nameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Please enter a username and password.");
        return;
    }

    bool success = databaseManager.signUpUser(username, password);

    if (!success) {
        QMessageBox::warning(this, "Sign Up Failed",
                             "Could not create account:\n" + databaseManager.getLastError());
        return;
    }

    statusLabel->setText("Status: Account created. You can log in now.");
    QMessageBox::information(this, "Success", "Account created successfully.");
}

void LoginWindow::onLoginButtonClicked()
{
    QString username = nameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString ip = ipEdit->text().trimmed();
    QString portText = portEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || ip.isEmpty() || portText.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Please enter username, password, server IP, and port.");
        return;
    }

    bool portOk = false;
    quint16 port = portText.toUShort(&portOk);

    if (!portOk) {
        QMessageBox::warning(this, "Invalid Port",
                             "Please enter a valid port number.");
        return;
    }

    StoredUser storedUser;

    bool success = databaseManager.loginUser(username, password, storedUser);

    if (!success) {
        QMessageBox::warning(this, "Login Failed",
                             databaseManager.getLastError());
        return;
    }

    pendingName = storedUser.username;

    statusLabel->setText("Status: Connecting...");
    loginButton->setEnabled(false);
    signUpButton->setEnabled(false);

    networkClient->connectToServer(ip, port);

    emit connectRequested(username, ip, port);
}

void LoginWindow::onConnectedToServer()
{
    UserService service;
    User currentUser = service.createUser(pendingName.toStdString());

    statusLabel->setText("Status: Logged in");

    DashboardWindow *dashboard = new DashboardWindow(currentUser, networkClient);
    dashboard->show();

    this->close();
}

void LoginWindow::onConnectionError(QString error)
{
    statusLabel->setText("Status: Failed - " + error);

    loginButton->setEnabled(true);
    signUpButton->setEnabled(true);

    QMessageBox::warning(this, "Connection Failed", error);
}