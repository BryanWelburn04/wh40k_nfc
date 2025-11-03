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
                waitForCardImproved(selectedReaderName, smartCardContext);
                //waitForCard(selectedReaderName, smartCardContext);
            } else {
                cout << "No valid reader selected." << endl;
            }
        }
       
        SCardReleaseContext(smartCardContext);
    }

    cout << "\n----------------END----------------\n";
    return 0;   
}

/*
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
*/

void waitForCardImproved(string selectedReaderName, SCARDCONTEXT smartCardContext) {

    /* potentially improved card wait function. Aimed to removed card polling
     * and instead, wait to connect, connect, wait for disconnect. 
     * Uses SCardGetStatusChange winscard.h function 
     * NEEDS the reader name passed as a pointer.
    */
     // Please see SCARD_READERSTATEA structure (winscard.h) Not sure how to intialize these
    
    // IN C this would have to be dynamically allocated, in C++ dont need to.
    // IM not sure if this needs to be initialized or just create struct and pass to stateChange() function
    // There is a few members to this struct, i think these ones need to be set beforehand?
    
    bool readerConnectionStatus = false;
    SCARDHANDLE			hCardHandle = 0;
	DWORD				uActiveProtocol = 0;	


    SCARD_READERSTATE readerState0 = {0}; // initalize struct to 0 otherwises throws error
    readerState0.szReader = selectedReaderName.c_str(); // Have to type cast this to LPCSTR (same as char*) from string
    readerState0.dwEventState = 0; //initialize to 0
    readerState0.dwCurrentState = SCARD_STATE_UNAWARE; //initialize state to unaware

    setStateForGetStatusChange(smartCardContext, readerState0); // initialize the state
    readerState0.dwCurrentState = readerState0.dwEventState;

    /* Why do we initialize with custom function setStateForGetStatusChange, then immediately call ScardGetStatusChange?
        They both use SCardGetStatuschange() so they do the same thing twice as far as I can tell.
        There must be a way to manually set the state, I see that the struct is given all 0's for member values.
            I can see that we have manually set dwCurrentstate and dwEventState to different things.
                WHy not initilaize them to begin equally, that way when we first enter into SCardGetStatusChange, they are equal so it blocks until changed. */

    LONG cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, &readerState0, 1); // infinite = timeout time (ms), 1 = length of readerStates
    
     // Now check if there is a card present in the reader
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        exit(1);
    }

    /*
    we have to use bitwise check because the state is a bitmasked 32bit int.
    basically the state will return as an 8bit number 0x00000000 and we are checking a particular bit in that number.
    SCARD_STATE_PRESENT = 0x00000020 but depending on our other states == will evalulate to false. so we have to use &.
    took me an hour to figure this out -_-.
    */
    if (readerState0.dwEventState & SCARD_STATE_PRESENT) { 
        // NOW we can connect to SCard, since there is one in the reader
        readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);
        if(readerConnectionStatus){
            getCardDetails(smartCardContext, selectedReaderName, hCardHandle, uActiveProtocol);
        }
        readerState0.dwCurrentState = readerState0.dwEventState; // copy new reader state to applications understanding of the state
        cout << "Card read successfully" << endl;

    } else {
        cout << "not reading as SCARD_STATE_PRESENT." << endl;
    }

    setStateForGetStatusChange(smartCardContext, readerState0);
    readerState0.dwCurrentState = readerState0.dwEventState;

    // NOW we must wait for card to be removed.
    cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, &readerState0, 1);
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        exit(1);
    }
    
    if (readerState0.dwEventState & SCARD_STATE_EMPTY) {
        cout << "card removed." << endl;
        SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
        readerState0.dwCurrentState = readerState0.dwEventState;
    } else {
        cout << "something went wrong. not SCARD_STATE_EMPTY" << endl;
    }
    
    //UNFINISHED!!!! CONTINUE FROM HERE.
    //NEED TO TEST THIS SO FAR BUT CANT.

} 

void setStateForGetStatusChange(SCARDCONTEXT smartCardContext, SCARD_READERSTATE &readerState0) {
    // We basically need to run this once to make the system aware of the current state.
    // otherwise it will always think theres a change and not wait at the SCardGetStatusChange.
    // So everytime we want to check for a status change we need to run this first.
    // probably a better way of doing this but this is all i got for now.
    LONG status = SCardGetStatusChange(smartCardContext, 0, &readerState0, 1);
    if (status != SCARD_S_SUCCESS) {
        cout << "setting state failed" << status << endl;
        return;
    }
    return;
}


