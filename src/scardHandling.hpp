#ifndef SCARDHANDLING_H
#define SCARDHANDLING_H

bool establishContext(SCARDCONTEXT& readerList);
bool getReaderList(SCARDCONTEXT all_readers, char* &pReaderString);
bool connectToReader(std::string readerName, SCARDCONTEXT smartCardContext, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol);
void getCardUID(SCARDCONTEXT smartCardContext, std::string selectedReaderName, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol);
void displayMemoryContent(BYTE *cardData, unsigned char startPage, unsigned char endPage);

#endif