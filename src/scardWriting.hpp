#ifndef SCARDWRITING_H
#define SCARDWRITING_H

void writeDataToCard(unsigned char startPage, BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol);

#endif