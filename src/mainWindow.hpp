#pragma once

#include <QMainWindow>
#include "troopInfoFunctions.hpp"
#include "NFCReader.hpp"
#include "modelInfoWindow.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(NFCReader* readerA, NFCReader* readerB, QWidget *parent = nullptr);

private slots:
    void exchangeCardData();

private:
    NFCReader* readerA;
    NFCReader* readerB;

    ModelInfoWindow* modelInfoWindowA = nullptr;
    ModelInfoWindow* modelInfoWindowB = nullptr;
};
