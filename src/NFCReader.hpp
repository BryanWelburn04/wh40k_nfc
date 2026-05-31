#pragma once

#include <QString>
#include <winscard.h>
#include <array>

class NFCReader
{
public:

    NFCReader(const std::wstring& name);

    void initializeReader();
    bool waitForCard();
    bool waitForCardRemoval();

    bool readPage(unsigned char pageNum, BYTE *infoContainer);
    bool readPages(unsigned char start, unsigned char end, BYTE *cardData);
    QString getRawDataFromCard(BYTE *cardData, unsigned char startPage, unsigned char endPage);
    QString getCardUID();

    void writeDataToCard(unsigned char startPage, BYTE *dataForCard, size_t sizeOfData);
    void resetPageData(unsigned char startPage, unsigned char endPage);
    bool writeStatsToCard(BYTE *dataForCard, size_t sizeOfData);
    bool writeNameToCard(BYTE *dataForCard, size_t sizeOfData);
    bool writeLinkToCard(BYTE *dataForCard, size_t sizeOfData);
    bool writeGreatestAchievementToCard(BYTE *dataForCard, size_t sizeOfData);
    bool writeWorstAchievementToCard(BYTE *dataForCard, size_t sizeOfData);
    bool writeHistoryToCard(int *totalStats, size_t sizeOfData);

    const std::wstring& getName() const;
    void printInfo();

private:

    const std::wstring readerName;

    SCARDCONTEXT smartCardContext = 0;
    SCARD_READERSTATEW readerState{};

    SCARDHANDLE hCardHandle = 0;
    DWORD activeProtocol = 0;

    bool connected = false;
};