#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QWidget>
#include "user.h"

class QLabel;
class QPushButton;
class QVBoxLayout;

class DashboardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWindow(const User &user, QWidget *parent = nullptr);

private:
    User currentUser;
    QLabel *titleLabel;
    QPushButton *newRequestButton;
    QVBoxLayout *requestsLayout;


    void displayRequestCard(QString title, QString category, QString location, QString status);

private slots:
    void addRequestCard(QString title, QString category, QString location);
};

#endif // DASHBOARDWINDOW_H
