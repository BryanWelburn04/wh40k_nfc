#pragma once
#include <QObject>
#include <winscard.h>
#include "troopInfoFunctions.hpp"

class CardWaitThread : public QObject
{
    Q_OBJECT

public:
    CardWaitThread(const wchar_t* readerName);

public slots:
    void process();

signals:
    void cardDetected(Troop troop, BYTE *cardData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);

private:
    std::wstring readerName;
};