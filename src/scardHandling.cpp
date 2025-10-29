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