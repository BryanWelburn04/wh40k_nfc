#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>

using namespace std;

//probably going to split this up into write name, kills, deaths, etc later
//just trying to get it working rn
void writeDataToCard(unsigned char startPage, BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol) {
    
    if (startPage < 4 || startPage > 129) {
        cout << "Invalid start page number." << endl;
        return;
    }

    if(sizeOfData %4 != 0){
        cout << "Data must me compleate pages of 4 bytes." << endl;
        return;
    }

    int numberOfPages = int(sizeOfData) / 4; 
    cout << "Writing " << numberOfPages << " pages starting at page " << (int)startPage << endl;

    //SCARD_IO_REQUEST ioRequest{};
    SCARD_IO_REQUEST ioRequest;
	    ioRequest.dwProtocol = uActiveProtocol;
	    ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);
    
    BYTE curPage[16];
    for(int i = 0; i < numberOfPages; i++) {

        BYTE apdu[] = {
            0xFF, 0x00, 0x00, 0x00, 0x07,
            0xD4, 0x40, 0x01,
            0xA2,
            static_cast<BYTE>(startPage + i),
            dataForCard[i*4 + 0],
            dataForCard[i*4 + 1],
            dataForCard[i*4 + 2],
            dataForCard[i*4 + 3]
        };
        
        DWORD cardDataSize = sizeof(curPage);

        int32_t status = SCardTransmit(hCardHandle, &ioRequest,
                                    apdu, sizeof(apdu),
                                    NULL,curPage, 
                                    &cardDataSize);

        if (status != SCARD_S_SUCCESS){
            cout << "Failed to write card data" << endl;
        } 
    }
}

void resetPageData(unsigned char startPage, unsigned char endPage, SCARDHANDLE hCardHandle, DWORD uActiveProtocol) {
    
    if (startPage < 4 || startPage > 129) {
        cout << "Invalid start page number." << endl;
        return;
    }

    if (endPage < 4 || endPage > 129 || endPage < startPage) {
        cout << "Invalid end page number." << endl;
        return;
    }

    int numberOfPages = endPage - startPage + 1;

    //SCARD_IO_REQUEST ioRequest{};
    SCARD_IO_REQUEST ioRequest;
	    ioRequest.dwProtocol = uActiveProtocol;
	    ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);
    
    BYTE curPage[16];
    for(int i = 0; i < numberOfPages; i++) {

        BYTE apdu[] = {
            0xFF, 0x00, 0x00, 0x00, 0x07,
            0xD4, 0x40, 0x01,
            0xA2,
            static_cast<BYTE>(startPage + i),
            0x00,
            0x00,
            0x00,
            0x00,
        };
        
        DWORD cardDataSize = sizeof(curPage);

        int32_t status = SCardTransmit(hCardHandle, &ioRequest,
                                    apdu, sizeof(apdu),
                                    NULL,curPage, 
                                    &cardDataSize);

        if (status != SCARD_S_SUCCESS){
            cout << "Failed to write card data" << endl;
        } 
    }
}