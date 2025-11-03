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
    // IM not sure if this needs to be initilized or just create struct and pass to statueChange() function
    SCARD_READERSTATE readerState1;
    readerState1.szReader = selectedReaderName.
    readerState1.pvUserData = NULL; // not sure what goes here maybe 0?
    readerState1.dwCurrentState = SCARD_STATE_EMPTY;
    readerState1.dwEventState = NULL; // this is state as known by smart card resource manager.

    SCARD_READERSTATE readerStates[1]; // This must be an array of readerstate structs
    long cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, readerStates, 1) // infinite = timeout time (ms), 1 = length of readerStates
    
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        exit(1);
    }

    // NOW we can connect to SCard
    readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);


    UNFINISHED!!!! CONTINUE FROM HERE.
    NEED TO TEST THIS SO FAR BUT CANT.

} */



