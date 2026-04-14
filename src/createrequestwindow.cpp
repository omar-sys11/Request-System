#include "createrequestwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>

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

    submitButton = new QPushButton("Submit Request", this);
    submitButton->setMinimumHeight(35);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Title:", titleEdit);
    formLayout->addRow(categoryLabel, categoryComboBox);
    formLayout->addRow(locationLabel, locationEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(submitButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setWindowTitle("Create Request");
    resize(400, 250);

    connect(submitButton, &QPushButton::clicked,
            this, &CreateRequestWindow::onSubmitClicked);
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
