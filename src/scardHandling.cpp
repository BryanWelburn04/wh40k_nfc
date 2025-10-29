#include <cstdio>
#include <iostream>
#include <winscard.h>
#include "scardHandling.hpp"

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

/*
getReaderList(...)

//code found in one of the sameple files, might be what we need? No idea what the parameters are

    int iResult = SCardListReaders(all_readers,NULL,(LPTSTR)&pReaderString, &cch );
	if (iResult != SCARD_S_SUCCESS){

*/