#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include <vector>


using namespace std;

void readPages(unsigned char startPage, unsigned char endPage, SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *cardData) {
    // MM: We pass in a pointer to cardData as a parameter, this means we need to initialize it prior to using
    //  it as a parameter. Why not initialize it here and return the reference to the caller?
    // i.e. BYTE cardData[6]; *do reading and put it in cardData* ; return &cardData;
    // However, I do like the fact that by passing a reference to cardData, we can instead return a success/failure code
    // This may be a better choice because it can make our error handling easier perhaps.
    // If SCardTransmit FAILS, it simply returns which will result in cardData being full of garbage which could be bad.
    // ANYWAY DELETE THIS WHEN YOU SEE IT leave a choice so I know tho: (return error codes OR return reference)
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

bool readTroopInfo(SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *troopInfo) {
    // Reads troop information page i.e. page 5
    int page = 5; // TROOPINFOPAGE ; global name for it later
}