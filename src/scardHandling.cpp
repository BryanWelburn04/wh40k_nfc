#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include "scardHandling.hpp"

using namespace std;

bool establishContext(SCARDCONTEXT& readerList) {
    long test = SCardEstablishContext(SCARD_SCOPE_USER,NULL,NULL,&readerList);
    printf("established context = %ld\n", test);
    printf("SCARD_S_SUCCESS = %ld\n",SCARD_S_SUCCESS);
    
    if (test != SCARD_S_SUCCESS) {
        //probably good idea to implement actually error handling
        printf("Failed\n");
        return false;
    } else {
        printf("Success\n");
        return true;
    }
    return false;
}


bool getReaderList(SCARDCONTEXT smartCardContext, char* &pReaderString){

    DWORD cch = SCARD_AUTOALLOCATE;

    int32_t readerList = SCardListReaders(smartCardContext,NULL,(LPTSTR)&pReaderString, &cch);
	if (readerList != SCARD_S_SUCCESS){
        cout << "Failed to get reader list" << endl;
    }
    string reader = pReaderString;
    if(reader != "" && reader != "Windows Hello for Business 1"){
        return true;
    } else {
        return false;
    }
    return false;
}

bool connectToReader(string readerName, SCARDCONTEXT smartCardContext, SCARDHANDLE &hCardHandle_, DWORD &uActiveProtocol_){

    int32_t readerConnection = SCardConnect(smartCardContext,readerName.data(),
                                                SCARD_SHARE_SHARED,SCARD_PROTOCOL_T1,
                                                &hCardHandle_,
                                                &uActiveProtocol_);

    if(readerConnection != SCARD_S_SUCCESS){
        cout << "Failed to connect to reader: " << readerName << endl;
        cout << "Handle: " << hCardHandle_ << " Protocol: " << uActiveProtocol_ << endl;
        return false;
    } else {
        return true;
    }
    return false;
}

void getCardUID(SCARDCONTEXT smartCardContext, string selectedReaderName, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol){
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
        for (int i = 0; i < cardDataSize-2; i++){
            printf("%02X ", cardData[i]);
        }
        cout << "\nStatus Bytes:" << endl;


        BYTE statusBytesindexs[2]; //this garbage is to make it work for cards with shorter uids. probably unnessecary but i didnt want to hard code it
        int counter = 0;
        for(int i = cardDataSize-2; i<cardDataSize; i++){
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
