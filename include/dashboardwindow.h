#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include "user.h"

class DashboardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWindow(const User& user, QWidget *parent = nullptr);

private:
    User currentUser;  // add this
    QLabel *titleLabel;
    QPushButton *newRequestButton;
    QVBoxLayout *requestsLayout;
};

#endif // DASHBOARDWINDOW_H
