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
    string selectedReaderName;

    bool contextStatus = establishContext(smartCardContext); 

    if(contextStatus){

        char *pReaderString  = NULL;

        // MAYBE: we might want to make this function has a return code so we can check its success?
        getReaderList(smartCardContext, pReaderString);

        if(pReaderString != NULL){
            printf("List of readers: %s\n", pReaderString);

            string selectedReaderName = string(pReaderString);
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

/*
void readerStateInit(SCARD_READERSTATE &readerstate, char *readerName) {
    // MM : this could be the way we initialize readers, could add extra parameters to control initialization.
    //  for now we can just hardcode the basic init.

    // CONCEPTUAL USE:
    //  SCARD_READERSTATE readerState0;
    //  readerStatInit(readerState, selectedReaderName);
    //  printf("selected reader name: %s\n", readerState0.szReader);
    // OUTPUT: selected reader name: ACR122U

    readerState = {0};
    readerState.szReader = readerName;
    readerState0.dwEventState = SCARD_STATE_EMPTY; 
    readerState0.dwCurrentState = SCARD_STATE_EMPTY; // SCARD_STATE_UNAWARE will result in an immediate return from SCardGetStatusChange()
    // Maybe we try using state_empty as the initial value? if its assumes its empty and it is then it waits, if it is not empty, then it detects and updates EventStatus.
} */



void waitForCardImproved(string selectedReaderName, SCARDCONTEXT smartCardContext) {

    /* potentially improved card wait function. Aimed to removed card polling
     * and instead, wait to connect, connect, wait for disconnect. 
     * Uses SCardGetStatusChange winscard.h function 
     * NEEDS the reader name passed as a pointer (c type string).
    */

    /* MM : This function does too much, we should split intialization and waiting functionalities. */
    
    bool readerConnectionStatus = false;
    SCARDHANDLE			hCardHandle = 0;
	DWORD				uActiveProtocol = 0;	


    SCARD_READERSTATE readerState0 = {0}; // initalize struct to 0 otherwises throws error
    readerState0.szReader = selectedReaderName.c_str(); // Have to type cast this to LPCSTR (same as char*) from string
    
    // MM : In this program we go from C_string -> string -> C_string, since this function wants a c type string, and we only ever casted to c++ string 
    //  to use cout for printing to screen, seems redundant. Maybe we should just leave it as a c type string and use printf().
    //      sidenote: I think the typecast to c++ string was just so the string could be easily compared to another string. 

    readerState0.dwEventState = 0; 
    readerState0.dwCurrentState = SCARD_STATE_UNAWARE;

    setStateForGetStatusChange(smartCardContext, readerState0); // initialize the state
    readerState0.dwCurrentState = readerState0.dwEventState;

    /* MM : Why do we initialize with custom function setStateForGetStatusChange, then immediately call ScardGetStatusChange?
        They both use SCardGetStatuschange() so they do the same thing twice as far as I can tell.
        There must be a way to manually set the state, I see that the struct is given all 0's for member values.
            I can see that we have manually set dwCurrentstate and dwEventState to different things.
                WHy not initilaize them to begin equally, that way when we first enter into SCardGetStatusChange, they are equal so it blocks until changed. */

    LONG cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, &readerState0, 1); // infinite = timeout time (ms), 1 = length of readerStates
    
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
        // Connect to SCard, since there is one in the reader
        readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);
        if(readerConnectionStatus){
            getCardDetails(smartCardContext, selectedReaderName, hCardHandle, uActiveProtocol);
        }
        readerState0.dwCurrentState = readerState0.dwEventState; // copy new reader state to applications understanding of the state
        cout << "Card read successfully" << endl;

    } else {
        cout << "not reading as SCARD_STATE_PRESENT." << endl;
    }

    setStateForGetStatusChange(smartCardContext, readerState0); // MM : again this feels strange as we just set card states to match.
    readerState0.dwCurrentState = readerState0.dwEventState;

    // wait for card to be removed.
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
    return;
} 

void setStateForGetStatusChange(SCARDCONTEXT smartCardContext, SCARD_READERSTATE &readerState0) {
    // We basically need to run this once to make the system aware of the current state.
    // otherwise it will always think theres a change and not wait at the SCardGetStatusChange.
    // So everytime we want to check for a status change we need to run this first.
    // probably a better way of doing this but this is all i got for now.

    // MM : Why dont we just initialize the states to be equal, I notice you set dwEventState = 0,
    //  why not make it start in the same state as dwCurrentState?
    LONG status = SCardGetStatusChange(smartCardContext, 0, &readerState0, 1);
    if (status != SCARD_S_SUCCESS) {
        cout << "setting state failed" << status << endl;
        return;
    }
    return;
}


