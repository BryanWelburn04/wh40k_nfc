#ifndef MAIN_H
#define MAIN_H

int main();
void waitForCard (char* selectedReaderName, SCARDCONTEXT smartCardContext, SCARD_READERSTATE readerState0, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol);
void setStateForGetStatusChange(SCARDCONTEXT smartCardContext, SCARD_READERSTATE &readerState0);
void readerStateInit(SCARD_READERSTATE &readerstate, char *readerName);
void cardOperations(SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol);


#endif