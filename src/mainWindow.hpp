#pragma once

#include <QMainWindow>
#include "troopInfoFunctions.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const wchar_t* selectedReaderName, QWidget *parent = nullptr);

private slots:
    Troop getTroopInfoFromCard();

private:
    std::wstring readerName;
};
