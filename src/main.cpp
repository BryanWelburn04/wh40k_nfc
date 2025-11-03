#include <cstdio>
#include <iostream>
#include <winscard.h>
#include "main.hpp"
#include "scardHandling.hpp"
#include <string>

using namespace std;

int main() {
    cout << "---------------START---------------\n\n";

    SCARDCONTEXT smartCardContext; // Winscard context
    string selectedReaderName;     //currently selected reader name

    bool contextStatus = establishContext(smartCardContext); 

    if(contextStatus){

        char *pReaderString  = NULL;

        // MAYBE: we might want to make this function has a return code so we can check its success?
        getReaderList(smartCardContext, pReaderString); // Establish reader list

        if(pReaderString != NULL){
            printf("List of readers: %s\n", pReaderString);

            string selectedReaderName = string(pReaderString);
            // could maybe make this cast then do cout << list of readers: << selectedReaderName << endl; instead of doing printf, then casting.

            if(selectedReaderName != "" && selectedReaderName != "Windows Hello for Business 1"){
                cout << "Connection to reader " << selectedReaderName << " was established" << endl;
                cout << "Waiting for card..." << endl;
                waitForCard(selectedReaderName, smartCardContext);
            } else {
                cout << "No valid reader selected." << endl;
            }
        }
       
        SCardReleaseContext(smartCardContext);
    }

    cout << "\n----------------END----------------\n";
    return 0;   
}

void waitForCard (string selectedReaderName, SCARDCONTEXT smartCardContext){
    bool readerConnectionStatus = false;
    SCARDHANDLE			hCardHandle;
	DWORD				uActiveProtocol;	

    while(1) {
        readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);
        if (readerConnectionStatus){
            getCardDetails(smartCardContext, selectedReaderName, hCardHandle, uActiveProtocol);
            while (readerConnectionStatus) {
                readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);
            }
            SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
            cout << "Waiting for card..." << endl;
        }
    }
}

/*
void waitForCardImproved(char *selectedReaderName, SCARDCONTEXT smartCardContext) {
    /* potentially improved card wait function. Aimed to removed card polling
     * and instead, wait to connect, connect, wait for disconnect. 
     * Uses SCardGetStatusChange winscard.h function 
     * NEEDS the reader name passed as a pointer.

     // Please see SCARD_READERSTATEA structure (winscard.h) Not sure how to intialize these
    
    // IN C this would have to be dynamically allocated, in C++ dont need to.
    // IM not sure if this needs to be initialized or just create struct and pass to stateChange() function
    // There is a few members to this struct, i think these ones need to be set beforehand?
    SCARD_READERSTATE readerState0;
    readerState0.szReader = selectedReaderName.
    readerState0.dwCurrentState = SCARD_STATE_EMPTY;

    SCARD_READERSTATE readerStates[1]; // This must be an array of readerstate structs
    readerStates[0] = readerState0;
    long cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, readerStates, 1); // infinite = timeout time (ms), 1 = length of readerStates
    
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        exit(1);
    }

    if (readerState0.dwEventState == SCARD_STATE_PRESENT) {
        // NOW we can connect to SCard, since there is one in the reader
        readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);
        readerState0.dwCurrentState = readerState0.dwEventState; // copy new reader state to applications understanding of the state
    } else {
        cout << "not reading as SCARD_STATE_PRESENT." << endl;
    }


    // NOW we must wait for card to be removed.
    cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, readerStates, 1);
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        exit(1);
    }
    
    if (readerState0.dwEventState == SCARD_STATE_EMPTY) {
        cout << "card removed." << endl;
        SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
        readerState0.dwCurrentState = readerState0.dwEventState;
    } else {
        cout << "something went wrong. not SCARD_STATE_EMPTY" << endl;
    }


    UNFINISHED!!!! CONTINUE FROM HERE.
    NEED TO TEST THIS SO FAR BUT CANT.

} */



