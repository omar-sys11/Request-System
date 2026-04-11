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
    FeedWindow(QWidget *parent = nullptr);

private slots:
    void onAddClicked();

private:
    FeedBoard board;

    QComboBox* typeBox;
    QLineEdit* senderInput;
    QLineEdit* contentInput;

    QVBoxLayout* feedLayout;
    QWidget* feedContainer;
    QScrollArea* scrollArea;

    void refreshFeed();
    QWidget* createCard(const FeedItem* item);
};
