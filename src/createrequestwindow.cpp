#include "createrequestwindow.h"
#include "OpenAiAPIClient.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>
#include <QTextEdit>
#include <QFrame>

CreateRequestWindow::CreateRequestWindow(QWidget *parent)
    : QWidget(parent)
{
    titleLabel = new QLabel("Create New Request", this);
    categoryLabel = new QLabel("Category:", this);
    locationLabel = new QLabel("Location:", this);
    statusLabel = new QLabel("Fill in the request details.", this);
    aiQuestionLabel = new QLabel("Ask AI TA a question:", this);

    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    titleEdit = new QLineEdit(this);
    locationEdit = new QLineEdit(this);

    categoryComboBox = new QComboBox(this);
    categoryComboBox->addItem("Study Help");
    categoryComboBox->addItem("Study Partner");
    categoryComboBox->addItem("Borrow Item");
    categoryComboBox->addItem("Quick Question");
    categoryComboBox->addItem("Tech Help");

    submitButton = new QPushButton("Submit Request", this);
    submitButton->setMinimumHeight(35);

    openAiClient = new OpenAiApiClient(this);

    aiQuestionEdit = new QTextEdit(this);
    aiQuestionEdit->setPlaceholderText("Example: How should I write a good request?");
    aiQuestionEdit->setMinimumHeight(70);

    askAiButton = new QPushButton("Ask AI TA", this);
    askAiButton->setMinimumHeight(35);

    aiResponseBox = new QFrame(this);
    aiResponseBox->setObjectName("aiResponseBox");

    aiNameLabel = new QLabel("AI TA", aiResponseBox);
    aiNameLabel->setObjectName("aiNameLabel");

    aiTextLabel = new QLabel("", aiResponseBox);
    aiTextLabel->setObjectName("aiTextLabel");
    aiTextLabel->setWordWrap(true);

    QVBoxLayout *aiBoxLayout = new QVBoxLayout(aiResponseBox);
    aiBoxLayout->addWidget(aiNameLabel);
    aiBoxLayout->addWidget(aiTextLabel);

    aiResponseBox->setLayout(aiBoxLayout);
    aiResponseBox->hide();

    aiResponseBox->setStyleSheet(
        "#aiResponseBox {"
        "   background-color: #d8ecff;"
        "   border: 2px solid #4a90e2;"
        "   border-radius: 10px;"
        "   padding: 8px;"
        "}"
        "#aiNameLabel {"
        "   color: #0b4f9c;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "}"
        "#aiTextLabel {"
        "   color: #003b73;"
        "   font-size: 13px;"
        "}"
    );

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Title:", titleEdit);
    formLayout->addRow(categoryLabel, categoryComboBox);
    formLayout->addRow(locationLabel, locationEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(aiQuestionLabel);
    mainLayout->addWidget(aiQuestionEdit);
    mainLayout->addWidget(askAiButton);
    mainLayout->addWidget(aiResponseBox);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(submitButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setWindowTitle("Create Request");
    resize(500, 500);

    connect(submitButton, &QPushButton::clicked,
            this, &CreateRequestWindow::onSubmitClicked);

    connect(askAiButton, &QPushButton::clicked,
            this, &CreateRequestWindow::onAskAiClicked);

    connect(openAiClient, &OpenAiApiClient::answerReceived,
            this, &CreateRequestWindow::onAiAnswerReceived);

    connect(openAiClient, &OpenAiApiClient::apiError,
            this, &CreateRequestWindow::onApiError);
}

void CreateRequestWindow::onSubmitClicked()
{
    QString title = titleEdit->text().trimmed();
    QString location = locationEdit->text().trimmed();
    QString category = categoryComboBox->currentText();

    if (title.isEmpty() || location.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Please fill in all required fields.");
        return;
    }

    emit requestSubmitted(title, category, location);

    statusLabel->setText("Request submitted successfully.");
    QMessageBox::information(this, "Success",
                             "Your request was submitted.");

    this->close();
}

void CreateRequestWindow::onAskAiClicked()
{
    QString question = aiQuestionEdit->toPlainText().trimmed();

    if (question.isEmpty()) {
        QMessageBox::warning(this, "Missing Question",
                             "Please type a question for AI TA.");
        return;
    }

    aiTextLabel->setText("Thinking...");
    aiResponseBox->show();

    statusLabel->setText("Asking AI TA...");
    askAiButton->setEnabled(false);

    openAiClient->askQuestion(question);
}

void CreateRequestWindow::onAiAnswerReceived(const QString &answer)
{
    aiTextLabel->setText(answer.toHtmlEscaped().replace("\n", "<br>"));
    aiResponseBox->show();

    statusLabel->setText("AI TA answered your question.");
    askAiButton->setEnabled(true);
}

void CreateRequestWindow::onApiError(const QString &message)
{
    aiTextLabel->setText(
        "Sorry, I could not get a response right now.<br><br>" +
        message.toHtmlEscaped().replace("\n", "<br>")
    );

    aiResponseBox->show();

    statusLabel->setText("AI TA request failed.");
    askAiButton->setEnabled(true);
}