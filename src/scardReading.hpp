#ifndef SCARDREADING_H
#define SCARDREADING_H
#include <vector>
#include <windows.h>
#include <winscard.h>
#include <QString>
#include <array>

bool readPages(unsigned char start, unsigned char end, SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *cardData);
bool readPage(short pageNum, SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *infoContainer);
void getCardUID(SCARDCONTEXT smartCardContext, const wchar_t* selectedReaderName, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol);
void displayMemoryContent(BYTE *cardData, unsigned char startPage, unsigned char endPage);
QString getRawDataFromCard(BYTE *cardData, unsigned char startPage, unsigned char endPage);
bool readTroopInfo(SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *infoContainer);
bool readTroopIdInfo(SCARDHANDLE hCardHandle, DWORD uActiveProtocol,  BYTE *idContainer);
bool readTroopName(SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *nameContainer);
std::array<int, 4> getHistoricTotals(SCARDHANDLE hCardHandle, DWORD uActiveProtocol);


#endif