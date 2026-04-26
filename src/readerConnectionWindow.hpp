#pragma once

#include <QMainWindow>
#include <QListWidget>

class ReaderConnectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    ReaderConnectionWindow(QWidget *parent = nullptr);

private slots:
    QList<QString> getAvailableReaders();
    void initializeReader();

private:
    QListWidget *readerListWidget;
};
