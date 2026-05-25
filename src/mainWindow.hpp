#pragma once

#include <QMainWindow>
#include "troopInfoFunctions.hpp"
#include "NFCReader.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(NFCReader* readerA, NFCReader* readerB, QWidget *parent = nullptr);

private slots:

private:
    NFCReader* readerA;
    NFCReader* readerB;
};
