#include "createrequestwindow.h"
#include "OpenAiApiClient.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>
#include <QFrame>

CreateRequestWindow::CreateRequestWindow(QWidget *parent)
    : QWidget(parent) {
    titleLabel = new QLabel("Create New Request", this);
    categoryLabel = new QLabel("Category:", this);
    locationLabel = new QLabel("Location:", this);
    statusLabel = new QLabel("Fill in the request details.", this);

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

    suggestCategoryButton = new QPushButton("Ask AI TA", this);
    suggestCategoryButton->setMinimumHeight(35);

    submitButton = new QPushButton("Submit Request", this);
    submitButton->setMinimumHeight(35);

    openAiClient = new OpenAiApiClient(this);

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
    mainLayout->addWidget(suggestCategoryButton);
    mainLayout->addWidget(aiResponseBox);
    mainLayout->addWidget(submitButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setWindowTitle("Create Request");
    resize(450, 350);

    connect(submitButton, &QPushButton::clicked,
            this, &CreateRequestWindow::onSubmitClicked);

    connect(suggestCategoryButton, &QPushButton::clicked,
            this, &CreateRequestWindow::onSuggestCategoryClicked);

    connect(openAiClient, &OpenAiApiClient::categorySuggested,
            this, &CreateRequestWindow::onCategorySuggested);

    connect(openAiClient, &OpenAiApiClient::apiError,
            this, &CreateRequestWindow::onApiError);
}

void CreateRequestWindow::onSubmitClicked() {
    QString title = titleEdit->text().trimmed();
    QString location = locationEdit->text().trimmed();

    if (title.isEmpty() || location.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Please fill in all required fields.");
        return;
    }

    emit requestCreated(
        title,
        categoryComboBox->currentText(),
        location
    );

    statusLabel->setText("Request submitted successfully.");

    QMessageBox::information(this, "Success",
                             "Your request was submitted.");
}

void CreateRequestWindow::onSuggestCategoryClicked() {
    QString title = titleEdit->text().trimmed();
    QString location = locationEdit->text().trimmed();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Missing Title",
                             "Please enter a request title before asking AI TA.");
        return;
    }

    aiTextLabel->setText("Thinking...");
    aiResponseBox->show();

    statusLabel->setText("Asking AI TA...");
    suggestCategoryButton->setEnabled(false);

    openAiClient->suggestCategory(title, location);
}

void CreateRequestWindow::onCategorySuggested(const QString &category) {
    int index = categoryComboBox->findText(category, Qt::MatchFixedString);

    if (index >= 0) {
        categoryComboBox->setCurrentIndex(index);

        aiTextLabel->setText(
            "I suggest using the category: <b>" + category + "</b>"
        );

        statusLabel->setText("AI TA suggested a category.");
    } else {
        categoryComboBox->setCurrentIndex(0);

        aiTextLabel->setText(
            "I could not match the AI response to one of the categories, "
            "so I selected <b>Study Help</b> as the default."
        );

        statusLabel->setText("AI TA response did not match a category.");
    }

    aiResponseBox->show();
    suggestCategoryButton->setEnabled(true);
}

void CreateRequestWindow::onApiError(const QString &message) {
    aiTextLabel->setText(
        "Sorry, I could not get a response right now.<br><br>" + message
    );

    aiResponseBox->show();

    statusLabel->setText("AI TA request failed.");
    suggestCategoryButton->setEnabled(true);
}
