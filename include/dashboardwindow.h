#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;

class DashboardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);

private:
    QLabel *titleLabel;
    QPushButton *newRequestButton;
    QVBoxLayout *requestsLayout;
};

#endif // DASHBOARDWINDOW_H
