#ifndef SCARDREADING_H
#define SCARDREADING_H
#include <vector>

bool readPages(unsigned char start, unsigned char end, SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *cardData);

#endif