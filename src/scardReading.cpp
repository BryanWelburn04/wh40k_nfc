#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include <vector>
#include <array>
#include <QString>

using namespace std;

bool readPages(unsigned char startPage, unsigned char endPage, SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *cardData) {
    
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
        BYTE pages[] = {0xFF, 0xB0, 0x00, static_cast<BYTE>(startPage+i), 0x04};
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
                cardData[i*4 + j] = curPage[j];
            }
        }
    }

    return true;
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

void getCardUID(SCARDCONTEXT smartCardContext, const wchar_t* selectedReaderName, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol){
    /* MM : Since this function is hardcoded to get card uid, it may be suitable to name it as such. My idea is that we can
        have a function for each "command" that we can use. This would be "getCardUID" command function.
            I imagine there is lots of commands but we may only want a handful of them so this could be suitable. */
    
    BYTE uid[] = { 0xFF, 0xCA, 0x00, 0x00, 0x00 }; // get uid command? found this online and it seems to work
    BYTE cardData[540];
    DWORD cardDataSize = sizeof(cardData);
    
    SCARD_IO_REQUEST ioRequest;
	    ioRequest.dwProtocol = uActiveProtocol;
	    ioRequest.cbPciLength = 8;

    int32_t status = SCardTransmit(hCardHandle, &ioRequest,
							    uid, sizeof(uid),
							    NULL,cardData, 
							    &cardDataSize);

	if (status != SCARD_S_SUCCESS){
        cout << "Failed to read card data" << endl;
        return;
    } else {
        cout << "Card UID:" << endl;
        for (int i = 0; i < static_cast<int>(cardDataSize-2); i++){
            printf("%02X ", cardData[i]);
        }
        cout << "\nStatus Bytes:" << endl;


        BYTE statusBytesindexs[2]; //this garbage is to make it work for cards with shorter uids. probably unnessecary but i didnt want to hard code it
        int counter = 0;
        for(int i = cardDataSize-2; i<static_cast<int>(cardDataSize); i++){
            printf("%02X ", cardData[i]);
            statusBytesindexs[counter] = i;
            counter++;
        }
        cout << endl;

        if(cardData[statusBytesindexs[0]] == 0x90 && cardData[statusBytesindexs[1]] == 0x00){ //0x90 0x00 means successful, anything else is error codes
            cout << "Success" << endl;
        } else {
            cout << "Error" << endl;
        }

    return;
    }
}

void displayMemoryContent(BYTE *cardData, unsigned char startPage, unsigned char endPage) {
    //this is gross but works
    cout << "Card Data:" << endl;
    int byteNumber = 0;
    int pageCounter = startPage;
    int numberOfBytes = (endPage - startPage + 1) * 4;
    for (int i = 0; i < numberOfBytes; i++) { 
        if (byteNumber%4 == 0){
            cout << endl;
            cout << pageCounter << ": ";
            pageCounter++;
        }
        printf("%02X ", cardData[i]);
        
        byteNumber++;
    }
    cout << endl;
}


QString getRawDataFromCard(BYTE *cardData, unsigned char startPage, unsigned char endPage) {
    QString result;
    int byteNumber = 0;
    int pageCounter = startPage;
    int numberOfBytes = (endPage - startPage + 1) * 4;
    for (int i = 0; i < numberOfBytes; i++) { 
        if (byteNumber%4 == 0){
            result += "\n" + QString::number(pageCounter) + ": ";
            pageCounter++;
        }
        result += QString("%1 ").arg(cardData[i], 2, 16, QChar('0')).toUpper();
        byteNumber++;
    }
    return result;
}


bool readTroopInfo(SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *infoContainer) {
    // Read troop info only, hardcoded page number but future use of a global page number is preffered.
    bool success_state = false;
    success_state = readPage(5, hCardHandle, uActiveProtocol, infoContainer);

    if (!success_state) {
        cout << "Encountered error in readTroopInfo()." << endl;
    }

    return success_state;
}

bool readTroopIdInfo(SCARDHANDLE hCardHandle, DWORD uActiveProtocol,  BYTE *idContainer) {
    // Read troop id info only, hardcoded page again. fix later.
    bool success_state = false;
    success_state = readPage(4, hCardHandle, uActiveProtocol, idContainer);

    if (!success_state) {
        cout << "Encountered error in readTroopIdInfo()." << endl;
    }

    return success_state;
}

bool readTroopName(SCARDHANDLE hCardHandle, DWORD uActiveProtocol, BYTE *nameContainer) {
    // Read troop name info only. max name length is 52. hardcode for now.
    bool success_state = false;
    success_state = readPages(0x07, 0x13, hCardHandle, uActiveProtocol, nameContainer);

    if (!success_state) {
        cout << "Encountered error in readTroopName()." << endl;
    } else {
        cout << "Troop Name: ";
        for (int i = 0; i < 52; i++) {
            if (nameContainer[i] == 0x00) { // 0 in ascii is null terminator, so we can stop reading the name once we hit it
                break;
            }
            printf("%c", (char)nameContainer[i]);
        }
        cout << endl;
    }

    

    return success_state;
}

std::array<int, 4> getHistoricTotals(SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
    
    int totalKills = 0;
    int totalDeaths = 0;
    int totalPrimaryPoints = 0;
    int totalSecondaryPoints = 0;

    std::array<int, 4> totals = {0, 0, 0, 0};

    for(int i = 0; i < 50; i++){
        BYTE gameData[4];
        readPage(80+i, hCardHandle, uActiveProtocol, gameData);
        totalKills += static_cast<int>(gameData[0]);
        totalDeaths += static_cast<int>(gameData[1]);
        totalPrimaryPoints += static_cast<int>(gameData[2]);
        totalSecondaryPoints += static_cast<int>(gameData[3]);
    }

    totals[0] = totalKills;
    totals[1] = totalDeaths;
    totals[2] = totalPrimaryPoints;
    totals[3] = totalSecondaryPoints;

    return totals;
}