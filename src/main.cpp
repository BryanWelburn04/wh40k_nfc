#include <cstdio>
#include <iostream>
#include <winscard.h>
#include "main.hpp"
#include "scardHandling.hpp"


int main() {
    printf("---------------START---------------\n\n");

    //winscard card list
    SCARDCONTEXT smartCardContext;
    bool status = establishContext(smartCardContext);
    if(status){
        //next step is get a list of all current readers
        //getReaderList(allReaders)... something like this?
        SCardReleaseContext(smartCardContext);
    }

    printf("\n----------------END----------------\n");
    return 0;   
}


