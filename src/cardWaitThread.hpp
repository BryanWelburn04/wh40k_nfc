#pragma once
#include <QObject>
#include <winscard.h>
#include "troopInfoFunctions.hpp"
#include "NFCReader.hpp"

class CardWaitThread : public QObject
{
    Q_OBJECT

public:
    CardWaitThread(NFCReader* reader);

public slots:
    void process();

signals:
    void cardDetected(Troop troop, BYTE *cardData);

private:
    NFCReader* reader;
};