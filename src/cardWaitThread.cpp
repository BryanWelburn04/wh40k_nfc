#include "cardWaitThread.hpp"
#include "scardHandling.hpp"
#include "troopInfoFunctions.hpp"
#include "scardReading.hpp"

CardWaitThread::CardWaitThread(NFCReader* reader) : reader(reader) {}

void CardWaitThread::process()
{
    // printf("CardWaitThread started for reader: %ls\n", this->readerName);

    // reader->initializeReader();

    while (true){
        reader->printInfo();
        bool status = reader->waitForCard();

        if(status){
        
            BYTE infoContainer[24];
            BYTE name[52];
            BYTE cardData[512];
            BYTE Link[148];
            BYTE ga[40];
            BYTE wa[40];

            //MAKE FUNCTIONS FOR THESE IN SCARDREADING.CPP/HPP
            reader->readPages(4, 9, infoContainer);
            reader->readPages(10, 22, name);
            reader->readPages(23, 58, Link);
            reader->readPages(59, 68, ga);
            reader->readPages(69, 78, wa);

            Troop troop = initTroop(name, infoContainer, ga, wa, Link);

            //needs to be after initTroop, idk why but it breaks everything if its before
            reader->readPages(0, 134, cardData);


            emit cardDetected(troop, cardData);
            reader->waitForCardRemoval();
        } else {
            Sleep(500);
        }
    }
}