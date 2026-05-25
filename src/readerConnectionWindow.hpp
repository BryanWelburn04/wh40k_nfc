#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QComboBox>

class ReaderConnectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    ReaderConnectionWindow(QWidget *parent = nullptr);

private slots:
    QList<QString> getAvailableReaders();
    void initializeReaders();

private:
    QListWidget *readerListWidget;
    QComboBox *readerABox;
    QComboBox *readerBBox;
};
