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

namespace scardHandling {

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
            scardHandling::readerStateInit(readerState0, selectedReaderName);
        } else {
            wcout << "Failed to establish connection to reader " << selectedReaderName << endl;
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
        scardHandling::setStateForGetStatusChange(smartCardContext, readerState0);
        cout << "waiting for card" << endl;
        
        // Copy the event state (updated in SCardGetStatusChange) to the current local state
        readerState0.dwCurrentState = readerState0.dwEventState;

        // Now we acctually call SCardGetStatusChange to wait
        cout << "Before waiting for card: " << readerState0.szReader << " state: " << readerState0.dwCurrentState << endl;
        LONG cardStatus = SCardGetStatusChange(smartCardContext, INFINITE, &readerState0, 1); // infinite = timeout time (ms), 1 = length of readerStates
        cout << "After waiting for card: " << readerState0.szReader << " state: " << readerState0.dwCurrentState << endl;

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

        scardHandling::setStateForGetStatusChange(smartCardContext, readerState0);
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

}