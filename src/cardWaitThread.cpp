#include "cardWaitThread.hpp"
#include "scardHandling.hpp"
#include "troopInfoFunctions.hpp"
#include "scardReading.hpp"
#include "constants.hpp"

CardWaitThread::CardWaitThread(NFCReader* reader) : reader(reader) {}

void CardWaitThread::process()
{
    // printf("CardWaitThread started for reader: %ls\n", this->readerName);

    // reader->initializeReader();

    while (true){
        reader->printInfo();
        bool status = reader->waitForCard();

        if(status){
        
            BYTE infoContainer[32];
            BYTE name[52];
            BYTE cardData[512];
            BYTE Link[132];
            BYTE ga[40];
            BYTE wa[40];

            //MAKE FUNCTIONS FOR THESE IN SCARDREADING.CPP/HPP
            reader->readPages(STATS_START_PAGE, STATS_END_PAGE, infoContainer);
            reader->readPages(NAME_START_PAGE, NAME_END_PAGE, name);
            reader->readPages(LINK_START_PAGE, LINK_END_PAGE, Link);
            reader->readPages(GA_START_PAGE, GA_END_PAGE, ga);
            reader->readPages(WA_START_PAGE, WA_END_PAGE, wa);

            Troop troop = initTroop(name, infoContainer, ga, wa, Link);

            //needs to be after initTroop, idk why but it breaks everything if its before
            reader->readPages(CARD_DATA_START_PAGE, CARD_DATA_END_PAGE, cardData);


            emit cardDetected(troop, cardData);
            reader->waitForCardRemoval();
        } else {
            Sleep(500);
        }
    }
}