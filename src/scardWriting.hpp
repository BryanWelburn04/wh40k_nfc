#ifndef SCARDWRITING_H
#define SCARDWRITING_H

#include <windows.h>
#include <winscard.h>

void writeDataToCard(unsigned char startPage, BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);
void resetPageData(unsigned char startPage, unsigned char endPage, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);

#endif