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


//pages 4-9
bool writeStatsToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
    if(sizeOfData > 24 || sizeOfData < 0){
        cout << "invalid data size for stats. DataSize = " << sizeOfData << endl;
        return false;
    }
    writeDataToCard(4, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
    return true;
}

//pages 10-22
bool writeNameToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
        if(sizeOfData > 52 || sizeOfData < 0){
        cout << "invalid data size for name. DataSize = " << sizeOfData << endl;
        return false;
    }
    writeDataToCard(10, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
    return true;
}

//pages 23-59
bool writeLinkToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
        if(sizeOfData > 148 || sizeOfData < 0){
        cout << "invalid data size for link. DataSize = " << sizeOfData << endl;
        return false;
    }
    writeDataToCard(23, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
    return true;
}

//pages 60-69
bool writeGreatestAchievementToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
        if(sizeOfData > 40 || sizeOfData < 0){
        cout << "invalid data size for greatest achievement. DataSize = " << sizeOfData << endl;
        return false;
    }
    writeDataToCard(60, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
    return true;
}

//pages 70-79
bool writeWorstAchievementToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
        if(sizeOfData > 40 || sizeOfData < 0){
        cout << "invalid data size for worst achievement. DataSize = " << sizeOfData << endl;
        return false;
    }
    writeDataToCard(70, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
    return true;
}

//pages 80-129
bool writeHistoryToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
        if(sizeOfData > 4 || sizeOfData < 0){
        cout << "invalid data size for history. DataSize = " << sizeOfData << endl;
        return false;
    }
    writeDataToCard(80, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
    return true;
}