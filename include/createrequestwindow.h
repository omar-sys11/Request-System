#ifndef CREATEREQUESTWINDOW_H
#define CREATEREQUESTWINDOW_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;

class CreateRequestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CreateRequestWindow(QWidget *parent = nullptr);

signals:
    void requestCreated(QString title, QString category, QString location);

private slots:
    void onSubmitClicked();

private:
    QLabel *titleLabel;
    QLabel *categoryLabel;
    QLabel *locationLabel;
    QLabel *statusLabel;

    QLineEdit *titleEdit;
    QComboBox *categoryComboBox;
    QLineEdit *locationEdit;

    QPushButton *submitButton;
};

#endif // CREATEREQUESTWINDOW_H
