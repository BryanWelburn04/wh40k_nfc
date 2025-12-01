#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include <vector>

using namespace std;

void readPages(unsigned char startPage, unsigned char endPage, SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *cardData) {
    int numberOfPages = endPage - startPage + 1;
    BYTE curPage[6];

    SCARD_IO_REQUEST ioRequest;
	    ioRequest.dwProtocol = uActiveProtocol;
	    ioRequest.cbPciLength = 8;

    for(int i = 0; i < numberOfPages; i++) {
        //this can read 1 block (min 4 bytes, max 16)
        //currently set to 4 so we can read it in chunks of 4 bytes instead of all 16 at once
        //runtime is horrible because of the number of sepreate calls to SCardTransmit
        //I'm sure there's a better way to do this
        BYTE pages[] = {0xFF, 0xB0, 0x00, startPage+i, 0x04};
        DWORD cardDataSize = sizeof(curPage);

        int32_t status = SCardTransmit(hCardHandle, &ioRequest,
                                    pages, sizeof(pages),
                                    NULL,curPage, 
                                    &cardDataSize);

        if (status != SCARD_S_SUCCESS){
            cout << "Failed to read card data" << endl;
            return;
        } else {
            for(int j = 0; j < 4; j++){
                cardData[i*4 + j] = curPage[j];
            }
        }
    }

    return;
}

bool readPage(short pageNum, SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *infoContainer) {
    // Reads page at pageNUM i.e. pageNum = 5 then read page 5.
    // Im noticing that ioRequest, command, status, and status check will be reoccuring alot
    //  for any reading instructions. Maybe we just make a single readPage funciton to read a single sepcified page.
    BYTE page = (BYTE)pageNum; // TROOPINFOPAGE ; global name for it later
    BYTE curPage[6];

    SCARD_IO_REQUEST ioRequest;
	    ioRequest.dwProtocol = uActiveProtocol;
	    ioRequest.cbPciLength = 8;

    BYTE pages[] = {0xFF, 0xB0, 0x00, page, 0x04};
    DWORD cardDataSize = sizeof(curPage);

    int32_t status = SCardTransmit(hCardHandle, &ioRequest,
                                pages, sizeof(pages),
                                NULL,curPage, 
                                &cardDataSize);
    
    if (status != SCARD_S_SUCCESS){
        cout << "Failed to read card data" << endl;
        return false;
    } else {
        for(int j = 0; j < 4; j++){
            infoContainer[j] = curPage[j];
        }
    }
    return true;
}