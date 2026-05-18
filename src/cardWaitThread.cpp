#include "cardWaitThread.hpp"
#include "scardHandling.hpp"
#include "troopInfoFunctions.hpp"
#include "scardReading.hpp"

CardWaitThread::CardWaitThread(const wchar_t* readerName) : readerName(readerName) {}

void CardWaitThread::process()
{
    SCARDCONTEXT smartCardContext;
    SCARD_READERSTATEW readerState0;
    SCARDHANDLE hCardHandle = 0;
    DWORD uActiveProtocol = 0;

    // printf("CardWaitThread started for reader: %ls\n", this->readerName);

    initializeReader(readerName.c_str(), smartCardContext, readerState0);

    while (true){
        waitForCard(readerName.c_str(), smartCardContext, readerState0, hCardHandle, uActiveProtocol);
        
        BYTE infoContainer[24];
        BYTE name[52];
        BYTE cardData[512];
        BYTE Link[148];
        BYTE ga[40];
        BYTE wa[40];

        //MAKE FUNCTIONS FOR THESE IN SCARDREADING.CPP/HPP
        readPages(4, 9, hCardHandle, uActiveProtocol, infoContainer);
        readPages(10, 22, hCardHandle, uActiveProtocol, name);
        readPages(23, 58, hCardHandle, uActiveProtocol, Link);
        readPages(59, 68, hCardHandle, uActiveProtocol, ga);
        readPages(69, 78, hCardHandle, uActiveProtocol, wa);

        Troop troop = initTroop(name, infoContainer, ga, wa, Link);

        //needs to be after initTroop, idk why but it breaks everything if its before
        readPages(0, 134, hCardHandle, uActiveProtocol, cardData);


        emit cardDetected(troop, cardData, hCardHandle, uActiveProtocol);
        waitForCardRemoval(smartCardContext, readerState0, hCardHandle);
        //Sleep(500);
    }
}