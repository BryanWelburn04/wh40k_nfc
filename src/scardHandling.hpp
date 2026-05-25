#ifndef SCARDHANDLING_H
#define SCARDHANDLING_H

#include <windows.h>
#include <winscard.h>

namespace scardHandling {

    wchar_t* getReaderList();
    void initializeReader(const wchar_t* selectedReaderName, SCARDCONTEXT &smartCardContext, SCARD_READERSTATEW &readerState0);
    bool waitForCard (const wchar_t* selectedReaderName, SCARDCONTEXT& smartCardContext, SCARD_READERSTATEW& readerState0, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol);
    bool waitForCardRemoval(SCARDCONTEXT &smartCardContext, SCARD_READERSTATEW &readerState0, SCARDHANDLE &hCardHandle);
    void setStateForGetStatusChange(SCARDCONTEXT &smartCardContext, SCARD_READERSTATEW &readerState0);
    void readerStateInit(SCARD_READERSTATEW& readerState, const wchar_t* readerName);
    bool establishContext(SCARDCONTEXT& readerList);
    bool getReaderList(SCARDCONTEXT all_readers, wchar_t* &pReaderString);
    bool connectToReader(const wchar_t* readerName, SCARDCONTEXT smartCardContext, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol);

}

#endif