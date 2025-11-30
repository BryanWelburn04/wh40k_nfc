#include <cstdio>
#include <iostream>
#include <winscard.h>
#include "main.hpp"
#include "scardHandling.hpp"
#include "scardReading.hpp"
#include "scardWriting.hpp"
#include <string>
#include <vector>

using namespace std;

int main() {
    cout << "---------------START---------------\n\n";

    SCARDCONTEXT smartCardContext;
    char* selectedReaderName;
    SCARD_READERSTATE readerState0;
    SCARDHANDLE	hCardHandle = 0;
	DWORD uActiveProtocol = 0;
    char *pReaderString  = NULL;


    bool contextStatus = establishContext(smartCardContext);  

    if(contextStatus){

        bool readerStatus = getReaderList(smartCardContext, pReaderString);
           
        if(readerStatus){

            printf("List of readers: %s\n", pReaderString);
            selectedReaderName = pReaderString;

            cout << "Connection to reader " << selectedReaderName << " was established" << endl;
            readerStateInit(readerState0, selectedReaderName);

            cout << "Waiting for card..." << endl;
            waitForCard(selectedReaderName, smartCardContext, readerState0, hCardHandle, uActiveProtocol);

        } else {
            cout << "No valid reader selected." << endl;
        }

        //close out everything (I'm definitely missing some, I haven't found all the SCARD free/close functions yet)
        SCardFreeMemory(smartCardContext, pReaderString);
        SCardReleaseContext(smartCardContext);
    }
    cout << "\n----------------END----------------\n";
    return 0;   
}

void readerStateInit(SCARD_READERSTATE &readerState, char *readerName) {
    
    readerState = {0};
    readerState.szReader = readerName;
    readerState.dwEventState = SCARD_STATE_UNAWARE; 
    readerState.dwCurrentState = SCARD_STATE_UNAWARE; 
}

void waitForCard(char* selectedReaderName, SCARDCONTEXT smartCardContext, SCARD_READERSTATE readerState0, SCARDHANDLE &hCardHandle, DWORD &uActiveProtocol) {
    
    bool readerConnectionStatus = false;
    BYTE cardData[540];


    /*
    * setStateForGetStatusChange is called to initalize the states for the reader.
    * States are currently locally set to UNAWARE
    * The first time SCardGetStatusChange runs it will automatically detect a chnage, update states, and return instantly
    * so it essentially has dual functionality of get status change and update states
    */
    setStateForGetStatusChange(smartCardContext, readerState0);
    
    // Copy the event state (updated in SCardGetStatusChange) to the current local state
    readerState0.dwCurrentState = readerState0.dwEventState;

    // Now we acctually call SCardGetStatusChange to wait
    LONG cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, &readerState0, 1); // infinite = timeout time (ms), 1 = length of readerStates
    
    if (cardStatus != SCARD_S_SUCCESS) {
        cout << "failed to wait for card." << endl;
        exit(1);
    }

    //we have to use bitwise check (&) because the state is a bitmasked 32bit int. Took me an hour to figure this out -_-.
    if (readerState0.dwEventState & SCARD_STATE_PRESENT) {
        // readerConnectionStatus is used to update and return hCardHandle and uActiveProtocol
        readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext, hCardHandle, uActiveProtocol);
        if(readerConnectionStatus){
            getCardUID(smartCardContext, selectedReaderName, hCardHandle, uActiveProtocol);

// --------------------------- IN PROGRESS -------------------------------------------------------------------            
            //ideally we don't want to plug the numbers directly into the function call
            //but for testing purposes this works for now
            unsigned char startPage = 0;
            unsigned char endPage = 134;
            readPages(startPage, endPage, hCardHandle, uActiveProtocol, cardData); //cardData declared at the top of this fucntion

            //this is gross but works
            cout << "Card Data:" << endl;
            int byteNumber = 0;
            int numberOfBytes = (endPage - startPage + 1) * 4;
            for (int i = 0; i < numberOfBytes; i++) { 
                printf("%02X ", cardData[i]);
                if (byteNumber%4 == 3){
                    cout << endl;
                }
                byteNumber++;
            }
// --------------------------- IN PROGRESS -------------------------------------------------------------------            
       
        }
        readerState0.dwCurrentState = readerState0.dwEventState;
        cout << "Card read successfully" << endl;

    } else {
        cout << "not reading as SCARD_STATE_PRESENT." << endl;
    }

    // Wait for card removal section...

    setStateForGetStatusChange(smartCardContext, readerState0);
    readerState0.dwCurrentState = readerState0.dwEventState;

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
    // uses SCardGetStatusChange as a state updater
    LONG status = SCardGetStatusChange(smartCardContext, 0, &readerState0, 1);
    if (status != SCARD_S_SUCCESS) {
        cout << "setting state failed" << status << endl;
        return;
    }
    return;
}


