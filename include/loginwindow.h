#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void connectRequested(const QString& name, const QString& ip, quint16 port);

private slots:
    void onConnectButtonClicked();

private:
    QLabel *titleLabel;
    QLabel *nameLabel;
    QLabel *ipLabel;
    QLabel *portLabel;
    QLabel *statusLabel;

    QLineEdit *nameEdit;
    QLineEdit *ipEdit;
    QLineEdit *portEdit;

    QPushButton *connectButton;
};

#endif // LOGINWINDOW_H
