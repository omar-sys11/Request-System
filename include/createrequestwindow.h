#ifndef CREATEREQUESTWINDOW_H
#define CREATEREQUESTWINDOW_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QTextEdit;
class QFrame;
class OpenAiApiClient;

class CreateRequestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CreateRequestWindow(QWidget *parent = nullptr);

    signals:
        void requestSubmitted(QString title, QString category, QString location);

private slots:
    void onSubmitClicked();
    void onAskAiClicked();
    void onAiAnswerReceived(const QString &answer);
    void onApiError(const QString &message);

private:
    QLabel *titleLabel;
    QLabel *categoryLabel;
    QLabel *locationLabel;
    QLabel *statusLabel;
    QLabel *aiQuestionLabel;

    QLineEdit *titleEdit;
    QComboBox *categoryComboBox;
    QLineEdit *locationEdit;

    QTextEdit *aiQuestionEdit;

    QPushButton *submitButton;
    QPushButton *askAiButton;

    QFrame *aiResponseBox;
    QLabel *aiNameLabel;
    QLabel *aiTextLabel;

    OpenAiApiClient *openAiClient;
};

#endif // CREATEREQUESTWINDOW_H