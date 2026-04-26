#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include "troopInfoFunctions.hpp"

class ModelInfoWindow : public QMainWindow
{
    Q_OBJECT

public:
ModelInfoWindow(const std::wstring& readerName,
                const Troop& troop,
                BYTE *cardData,
                QWidget *parent = nullptr);
Troop troop;
BYTE cardData;
                
private slots:

private:
    std::wstring readerName;
    unsigned char startPage = 0;
    unsigned char endPage = 134;
    //Troop *troop;
};
