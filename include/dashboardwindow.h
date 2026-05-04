#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QWidget>
#include "user.h"
#include "RequestManager.h"
#include "networkclient.h"

class QLabel;
class QPushButton;
class QVBoxLayout;

class DashboardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWindow(const User &user, NetworkClient *client,  QWidget *parent = nullptr);

private slots:
    void onMessageReceived(QString type, QString requestId, QString title,
                           QString category, QString location,
                           QString poster, QString status);
    void onDisconnected();

private:
    User currentUser;
    NetworkClient *networkClient;

    QLabel *titleLabel;
    QPushButton *newRequestButton;
    QVBoxLayout *requestsLayout;

    void displayRequestCard(QString title, QString category, QString location, QString status);

private slots:
    void addRequestCard(QString title, QString category, QString location);
};

#endif
