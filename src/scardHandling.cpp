#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include <string>
#include <vector>
#include <sstream>
#include "scardHandling.hpp"
#include "scardReading.hpp"
#include "scardWriting.hpp"

using namespace std;

wchar_t* getReaderList() {

    wchar_t* pReaderString  = NULL;
    SCARDCONTEXT smartCardContext;

    bool contextStatus = establishContext(smartCardContext);  

    if(contextStatus){

        bool readerStatus = getReaderList(smartCardContext, pReaderString);
           
        if(readerStatus){
            return pReaderString;
        }
    }
    SCardFreeMemory(smartCardContext, pReaderString);
    return NULL;
}


void initializeReader(const wchar_t* selectedReaderName, SCARDCONTEXT &smartCardContext, SCARD_READERSTATEW &readerState0) {

    bool contextStatus = establishContext(smartCardContext);  

    if(contextStatus){
        wcout << "Connection to reader " << selectedReaderName << " was established" << endl;
        readerStateInit(readerState0, selectedReaderName);
    }
    return;
}

void readerStateInit(SCARD_READERSTATEW &readerState, const wchar_t* readerName) {
    
    readerState = {0};
    readerState.szReader = readerName;
    readerState.dwEventState = SCARD_STATE_UNAWARE; 
    readerState.dwCurrentState = SCARD_STATE_UNAWARE; 
}

bool waitForCard(const wchar_t* selectedReaderName, SCARDCONTEXT &smartCardContext, SCARD_READERSTATEW &readerState0, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol) {
    
    bool result = false;
    bool readerConnectionStatus = false;

    /*
    * setStateForGetStatusChange is called to initalize the states for the reader.
    * States are currently locally set to UNAWARE
    * The first time SCardGetStatusChange runs it will automatically detect a chnage, update states, and return instantly
    * so it essentially has dual functionality of get status change and update states
    */
    setStateForGetStatusChange(smartCardContext, readerState0);
    cout << "waiting for card" << endl;
    
    // Copy the event state (updated in SCardGetStatusChange) to the current local state
    readerState0.dwCurrentState = readerState0.dwEventState;

    // Now we acctually call SCardGetStatusChange to wait
    LONG cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, &readerState0, 1); // infinite = timeout time (ms), 1 = length of readerStates
    
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        return false;
    }

    //we have to use bitwise check (&) because the state is a bitmasked 32bit int. Took me an hour to figure this out -_-.
    if (readerState0.dwEventState & SCARD_STATE_PRESENT) {
        // readerConnectionStatus is used to update and return hCardHandle and uActiveProtocol
        readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);
        if(readerConnectionStatus){
            //getCardUID(smartCardContext, selectedReaderName, hCardHandle, uActiveProtocol);
            //cardOperations(hCardHandle, uActiveProtocol);
            //getRawDataFromCard();
            result = true;
        }
        readerState0.dwCurrentState = readerState0.dwEventState;
        cout << "Card read successfully" << endl;

    } else {
        cout << "not reading as SCARD_STATE_PRESENT." << endl;
        result = false;
    }
    return result;
}

bool waitForCardRemoval(SCARDCONTEXT &smartCardContext, SCARD_READERSTATEW &readerState0, SCARDHANDLE &hCardHandle) {
    // Wait for card removal section...

    setStateForGetStatusChange(smartCardContext, readerState0);
    readerState0.dwCurrentState = readerState0.dwEventState;

    LONG cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, &readerState0, 1);
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        return false;
    }
    
    if (readerState0.dwEventState & SCARD_STATE_EMPTY) {
        cout << "card removed." << endl;
        SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
        readerState0.dwCurrentState = readerState0.dwEventState;
    } else {
        cout << "something went wrong. not SCARD_STATE_EMPTY" << endl;
        return false;
    }
    return true;
} 

void setStateForGetStatusChange(SCARDCONTEXT &smartCardContext, SCARD_READERSTATEW &readerState0) {
    // uses SCardGetStatusChange as a state updater
    LONG status = SCardGetStatusChange(smartCardContext, 0, &readerState0, 1);
    if (status != SCARD_S_SUCCESS) {
        cout << "setting state failed" << status << endl;
        return;
    }
    return;
}

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


bool getReaderList(SCARDCONTEXT smartCardContext, wchar_t* &pReaderString){

    DWORD cch = SCARD_AUTOALLOCATE;

    int32_t readerList = SCardListReaders(smartCardContext,NULL,(LPTSTR)&pReaderString, &cch);
	if (readerList != SCARD_S_SUCCESS){
        cout << "Failed to get reader list" << endl;
    }
    wstring reader = pReaderString;
    if(reader != L"" && reader != L"Windows Hello for Business 1"){
        return true;
    } else {
        return false;
    }
    return false;
}

bool connectToReader(const wchar_t* readerName, SCARDCONTEXT smartCardContext, SCARDHANDLE &hCardHandle_, DWORD &uActiveProtocol_){

    int32_t readerConnection = SCardConnect(smartCardContext,readerName,
                                                SCARD_SHARE_SHARED,SCARD_PROTOCOL_T1,
                                                &hCardHandle_,
                                                &uActiveProtocol_);

    if(readerConnection != SCARD_S_SUCCESS){
        wcout << "Failed to connect to reader: " << readerName << endl;
        cout << "Handle: " << hCardHandle_ << " Protocol: " << uActiveProtocol_ << endl;
        return false;
    } else {
        return true;
    }
    return false;
}

/*
void getRawData(SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol){
    // --------------------------- IN PROGRESS -------------------------------------------------------------------            

    BYTE cardData[540]; // Passed to readPages function

    unsigned char startPage = static_cast<unsigned char>(startInt);
    unsigned char endPage = static_cast<unsigned char>(endInt);

        cout << "Reading pages " << startInt << " to " << endInt << endl;

        readPages(startPage, endPage, hCardHandle, uActiveProtocol, cardData); //cardData declared at the top of this function
        displayMemoryContent(cardData, startPage, endPage);

}
*/





//SHOULD NOT BE IN SCARDHANDLING!!
//SPLIT INTO READING WRITING ETC
void cardOperations(SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol){
    // --------------------------- IN PROGRESS -------------------------------------------------------------------            

    BYTE cardData[540]; // Passed to readPages function

    cout << "Press 'r' for read card data. Press 'w' for write data to card. Press 'n' for read troop name." << endl;
    char operation = getchar(); 
    std::cin.ignore(1, '\n'); // to clear the newline character from the input buffer

    if(operation == 'r'){
        
        /*
        * takes page range from the terminal in the format:
        * startPage-endPage
        */
        
        cout << "Enter startPage and endPage. EX/ 11-25" << endl;

        string pageRange;
        getline(cin, pageRange);

        int startInt, endInt;
        stringstream ss(pageRange);
        ss >> startInt;
        ss.ignore(1, '-');
        ss >> endInt;

        unsigned char startPage = static_cast<unsigned char>(startInt);
        unsigned char endPage = static_cast<unsigned char>(endInt);

        cout << "Reading pages " << startInt << " to " << endInt << endl;

        readPages(startPage, endPage, hCardHandle, uActiveProtocol, cardData); //cardData declared at the top of this function
        displayMemoryContent(cardData, startPage, endPage);

    }

    if(operation == 'w'){
        
        /*
        * takes start page and data from the terminal in the formaat:
        * startPage-x,x,x,x ... (has to be in sets of 4,8,12 etc)
        */

        cout << "Enter data to write to card (must be in blocks of 4). EX/ startPage-3,F,62,C" << endl;
        
        size_t capacity = 4;
        size_t length = 0;
        BYTE* dataToWrite = (BYTE*)malloc(capacity); // I suck at cpp dynamic memory allocation so i resorted back to c

        string inputData;
        getline(cin, inputData);

        stringstream ss(inputData);
        
        // gets start page number
        int startInt;
        ss >> startInt;
        ss.ignore(1, '-');
        unsigned char startPage = static_cast<unsigned char>(startInt);

        // gets the data after the dash
        int i = 0;
        unsigned char curChar;
        while(ss >> curChar){
            if(length >= capacity){
                capacity += 4;
                dataToWrite = (BYTE*)realloc(dataToWrite, sizeof(dataToWrite) + 4);
            }

            dataToWrite[i] = static_cast<BYTE>(curChar);
            cout << "curChar = " << curChar << endl;
            printf("%02X ", dataToWrite[i]);

            length++;

            if(ss.peek() == ','){
                ss.ignore(1);
            }
            i++;
        }

        cout << "dataSize = " << capacity << endl;
        if(startPage == 7){
            resetPageData(startPage, 19, hCardHandle, uActiveProtocol); // set page data back to 0x00
        }
        writeDataToCard(startPage, dataToWrite, capacity, hCardHandle, uActiveProtocol);
        free(dataToWrite);
    }

    if(operation == 'n'){
        readTroopName(hCardHandle, uActiveProtocol, cardData);
    }
// --------------------------- IN PROGRESS -------------------------------------------------------------------            
}