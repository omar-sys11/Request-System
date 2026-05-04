#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
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
    void onLoginButtonClicked();
    void onSignUpButtonClicked();

private:
    QLabel *titleLabel;
    QLabel *nameLabel;
    QLabel *passwordLabel;
    QLabel *statusLabel;

    QLineEdit *nameEdit;
    QLineEdit *passwordEdit;

    QPushButton *loginButton;
    QPushButton *signUpButton;

    DatabaseManager databaseManager;
};

#endif // LOGINWINDOW_H