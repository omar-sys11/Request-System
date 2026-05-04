#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "networkclient.h"
#include "databasemanager.h"

class QLabel;
class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

    signals:
        void connectRequested(const QString &name, const QString &ip, quint16 port);

private slots:
    void onConnectedToServer();
    void onConnectionError(QString error);
    void onLoginButtonClicked();
    void onSignUpButtonClicked();

private:
    QLabel *titleLabel;
    QLabel *nameLabel;
    QLabel *passwordLabel;
    QLabel *ipLabel;
    QLabel *portLabel;
    QLabel *statusLabel;

    QLineEdit *nameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *ipEdit;
    QLineEdit *portEdit;

    QPushButton *loginButton;
    QPushButton *signUpButton;

    NetworkClient *networkClient;
    QString pendingName;

    DatabaseManager databaseManager;
};

#endif // LOGINWINDOW_H