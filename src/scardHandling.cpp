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
        //probalbly good idea to implement actuall error handling
        printf("Failed\n");
        return false;
    } else {
        printf("Success\n");
        return true;
    }
    return false;
}


void getReaderList(SCARDCONTEXT smartCardContext, char* &pReaderString){

//code found in one of the sameple files, might be what we need?
    DWORD cch = SCARD_AUTOALLOCATE;

    int32_t readerList = SCardListReaders(smartCardContext,NULL,(LPTSTR)&pReaderString, &cch);
	if (readerList != SCARD_S_SUCCESS){
        std::cout << "Failed to get reader list" << std::endl;
    }
    return;
}

bool connectToReader(string readerName, SCARDCONTEXT smartCardContext){

    SCARDHANDLE			hCardHandle_;
	DWORD				uActiveProtocol_;	

        int32_t readerConnection = SCardConnect(smartCardContext,readerName.data(),
                                                SCARD_SHARE_SHARED,SCARD_PROTOCOL_T1,
                                                &hCardHandle_,
                                                &uActiveProtocol_);

        if(readerConnection != SCARD_S_SUCCESS){
            // cout << "Failed to connect to reader: " << readerName << endl;
            // cout << "Handle: " << hCardHandle_ << " Protocol: " << uActiveProtocol_ << endl;
            return false;
        } else {
            //cout << "Handle: " << hCardHandle_ << " Protocol: " << uActiveProtocol_ << endl;
            return true;
        }

    return false;
}

void getCardDetails(SCARDCONTEXT smartCardContext, string selectedReaderName){
    cout << "/* placeholder for card details */" << endl;
}
