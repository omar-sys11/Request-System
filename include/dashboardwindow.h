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
    explicit DashboardWindow(const User &user, NetworkClient *client, QWidget *parent = nullptr);

private slots:
    void addRequestCard(QString title, QString category, QString location);

    void onMessageReceived(QString type,
                           QString requestId,
                           QString title,
                           QString category,
                           QString location,
                           QString poster,
                           QString status);

    void onDisconnected();

private:
    User currentUser;
    NetworkClient *networkClient;
    RequestManager requestManager;

    QLabel *titleLabel;
    QPushButton *newRequestButton;
    QVBoxLayout *requestsLayout;

    void refreshRequests();
    void displayRequestCard(QString title, QString category, QString location, QString status);
};

#endif