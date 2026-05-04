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
    statusLabel = new QLabel("Status: Not logged in", this);

    nameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    ipEdit->setText("127.0.0.1");
    portEdit->setText("12345");	

    networkClient = new NetworkClient(this);
    connect(networkClient, &NetworkClient::connectedToServer,
            this, &LoginWindow::onConnectedToServer);
    connect(networkClient, &NetworkClient::connectionError,
            this, &LoginWindow::onConnectionError);
    loginButton = new QPushButton("Login", this);
    signUpButton = new QPushButton("Sign Up", this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(nameLabel, nameEdit);
    formLayout->addRow(passwordLabel, passwordEdit);

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
    resize(350, 220);

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

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Please enter a username and password.");
        return;
    }

    StoredUser storedUser;

    bool success = databaseManager.loginUser(username, password, storedUser);

    if (!success) {
        QMessageBox::warning(this, "Login Failed",
                             databaseManager.getLastError());
        return;
    }

    pendingName = name;
    statusLabel->setText("Status: Connecting...");
    connectButton->setEnabled(false);
    networkClient->connectToServer(ip, port);
}

void LoginWindow::onConnectedToServer() {
    // SIMPLE SERVICE CALL
    UserService service;
    User currentUser = service.createUser(pendingName.toStdString());

    statusLabel->setText("Status: Logged in");

    DashboardWindow *dashboard = new DashboardWindow(currentUser, networkClient);
    dashboard->show();

    this->close();
}

void LoginWindow::onConnectionError(QString error) {
    statusLabel->setText("Status: Failed - " + error);
    connectButton->setEnabled(true);
    QMessageBox::warning(this, "Connection Failed", error);
}
