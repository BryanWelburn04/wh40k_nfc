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
        SCardReleaseContext(allReaders);
    }

    printf("\n----------------END----------------\n");
    return 0;   
}

//    int iResult = SCardListReaders(all_readers,NULL,
//							   (LPTSTR)&pReaderString, &cch );
//	if (iResult != SCARD_S_SUCCESS)
//	{    printf("testing\n");
