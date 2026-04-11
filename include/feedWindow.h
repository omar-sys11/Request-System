#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

#include "FeedBoard.h"

class FeedWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FeedWindow(QWidget *parent = nullptr);

private slots:
    void onAddClicked();

private:
    FeedBoard board;

    // input UI
    QComboBox* typeBox;
    QLineEdit* senderInput;
    QLineEdit* contentInput;

    QPushButton* addButton;

    // feed UI
    QWidget* feedContainer;
    QVBoxLayout* feedLayout;
    QScrollArea* scrollArea;

    void refreshFeed();
    QWidget* createCard(const FeedItem* item);
};
