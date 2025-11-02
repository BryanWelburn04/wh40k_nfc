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

    bool status = establishContext(smartCardContext);

    if(status){

        char *pReaderString  = NULL;

        getReaderList(smartCardContext, pReaderString);

        if(pReaderString != NULL){
            cout << "List of readers: ";
            printf("%s\n", pReaderString);
        
            string selectedReaderName = string(pReaderString);

            if(selectedReaderName != "" && selectedReaderName != "Windows Hello for Business 1"){
                cout << "Connection to reader " << selectedReaderName << " was established" << endl;
                cout << "Waiting for card..." << endl;
                waitForCard(selectedReaderName, smartCardContext);
            } else {
                cout << "No valid reader selected." << endl;
            }
        }
       
        SCardReleaseContext(smartCardContext);
    }

    cout << "\n----------------END----------------\n";
    return 0;   
}

void waitForCard (string selectedReaderName, SCARDCONTEXT smartCardContext){
    bool readerConnectionStatus = false;
    while(1) {
        readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext);
        if (readerConnectionStatus){
            getCardDetails(smartCardContext, selectedReaderName);
            while (readerConnectionStatus) {
                readerConnectionStatus = connectToReader(selectedReaderName, smartCardContext);
            }
            cout << "Waiting for card..." << endl;
        }
    }
}



