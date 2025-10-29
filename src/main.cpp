#include <cstdio>
#include <iostream>
#include <winscard.h>
#include "main.hpp"
#include "scardHandling.hpp"


int main() {
    printf("---------------START---------------\n\n");

    //winscard card list
    SCARDCONTEXT allReaders;
    bool status = establishContext(allReaders);
    if(status){
        //next step is get a list of all current readers
        //getReaderList(allReaders)... something like this?
        SCardReleaseContext(allReaders);
    }

    printf("\n----------------END----------------\n");
    return 0;   
}


