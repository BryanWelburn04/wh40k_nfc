#ifndef SCARDWRITING_H
#define SCARDWRITING_H

#include <windows.h>
#include <winscard.h>

namespace scardWriting {

    void writeDataToCard(unsigned char startPage, BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
    void resetPageData(unsigned char startPage, unsigned char endPage, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
    bool writeStatsToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
    bool writeNameToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
    bool writeLinkToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
    bool writeGreatestAchievementToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
    bool writeWorstAchievementToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
    bool writeHistoryToCard(int *totalStats, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);

}

#endif