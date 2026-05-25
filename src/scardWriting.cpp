#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include "scardReading.hpp"

using namespace std;

namespace scardWriting {

    //probably going to split this up into write name, kills, deaths, etc later
    //just trying to get it working rn
    void writeDataToCard(unsigned char startPage, BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol) {
        
        if (startPage < 4 || startPage > 129) {
            cout << "Invalid start page number." << endl;
            return;
        }

        if(sizeOfData %4 != 0){
            cout << "Data must me compleate pages of 4 bytes." << endl;
            return;
        }

        int numberOfPages = int(sizeOfData) / 4; 
        cout << "Writing " << numberOfPages << " pages starting at page " << (int)startPage << endl;

        //SCARD_IO_REQUEST ioRequest{};
        SCARD_IO_REQUEST ioRequest;
            ioRequest.dwProtocol = uActiveProtocol;
            ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);
        
        BYTE curPage[16];
        for(int i = 0; i < numberOfPages; i++) {

            BYTE apdu[] = {
                0xFF, 0x00, 0x00, 0x00, 0x07,
                0xD4, 0x40, 0x01,
                0xA2,
                static_cast<BYTE>(startPage + i),
                dataForCard[i*4 + 0],
                dataForCard[i*4 + 1],
                dataForCard[i*4 + 2],
                dataForCard[i*4 + 3]
            };
            
            DWORD cardDataSize = sizeof(curPage);

            int32_t status = SCardTransmit(hCardHandle, &ioRequest,
                                        apdu, sizeof(apdu),
                                        NULL,curPage, 
                                        &cardDataSize);

            if (status != SCARD_S_SUCCESS){
                cout << "Failed to write card data" << endl;
            } 
        }
    }

    void resetPageData(unsigned char startPage, unsigned char endPage, SCARDHANDLE hCardHandle, DWORD uActiveProtocol) {
        
        if (startPage < 4 || startPage > 129) {
            cout << "Invalid start page number." << endl;
            return;
        }

        if (endPage < 4 || endPage > 129 || endPage < startPage) {
            cout << "Invalid end page number." << endl;
            return;
        }

        int numberOfPages = endPage - startPage + 1;

        //SCARD_IO_REQUEST ioRequest{};
        SCARD_IO_REQUEST ioRequest;
            ioRequest.dwProtocol = uActiveProtocol;
            ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);
        
        BYTE curPage[16];
        for(int i = 0; i < numberOfPages; i++) {

            BYTE apdu[] = {
                0xFF, 0x00, 0x00, 0x00, 0x07,
                0xD4, 0x40, 0x01,
                0xA2,
                static_cast<BYTE>(startPage + i),
                0x00,
                0x00,
                0x00,
                0x00,
            };
            
            DWORD cardDataSize = sizeof(curPage);

            int32_t status = SCardTransmit(hCardHandle, &ioRequest,
                                        apdu, sizeof(apdu),
                                        NULL,curPage, 
                                        &cardDataSize);

            if (status != SCARD_S_SUCCESS){
                cout << "Failed to write card data" << endl;
            } 
        }
    }


    //pages 4-9
    bool writeStatsToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
        if(sizeOfData > 24 || sizeOfData < 0){
            cout << "invalid data size for stats. DataSize = " << sizeOfData << endl;
            return false;
        }
        writeDataToCard(4, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
        return true;
    }

    //pages 10-22
    bool writeNameToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
            if(sizeOfData > 52 || sizeOfData < 0){
            cout << "invalid data size for name. DataSize = " << sizeOfData << endl;
            return false;
        }
        writeDataToCard(10, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
        return true;
    }

    //pages 23-58
    bool writeLinkToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
            if(sizeOfData > 144 || sizeOfData < 0){
            cout << "invalid data size for link. DataSize = " << sizeOfData << endl;
            return false;
        }
        writeDataToCard(23, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
        return true;
    }

    //pages 59-68
    bool writeGreatestAchievementToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
            if(sizeOfData > 40 || sizeOfData < 0){
            cout << "invalid data size for greatest achievement. DataSize = " << sizeOfData << endl;
            return false;
        }
        writeDataToCard(59, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
        return true;
    }

    //pages 69-78
    bool writeWorstAchievementToCard(BYTE *dataForCard, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){
            if(sizeOfData > 40 || sizeOfData < 0){
            cout << "invalid data size for worst achievement. DataSize = " << sizeOfData << endl;
            return false;
        }
        writeDataToCard(69, dataForCard, sizeOfData, hCardHandle, uActiveProtocol);
        return true;
    }

    //pages 79-129
    bool writeHistoryToCard(int *totalStats, size_t sizeOfData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol){

        cout << "Attempting to write to history with totalStats: " << totalStats[0] << ", " << totalStats[1] << ", " << totalStats[2] << ", " << totalStats[3] << endl;

        // if(sizeOfData > 4 || sizeOfData < 0){
        //     cout << "invalid data size for history. DataSize = " << sizeOfData << endl;
        //     return false;
        // }

        BYTE gamesPlayed[4];
        scardReading::readPage(79, hCardHandle, uActiveProtocol, gamesPlayed);
        int totalGamesPlayed = (static_cast<unsigned int>(gamesPlayed[0] << 24)) | (static_cast<unsigned int>(gamesPlayed[1] << 16)) | (static_cast<unsigned int>(gamesPlayed[2] << 8)) | static_cast<unsigned int>(gamesPlayed[3]);

        int currentGameStartPage = (totalGamesPlayed % 50) + 80; //this will write to pages 79-128 in a loop, so we can store the last 50 games played.
        int previousGameStartPage = ((totalGamesPlayed - 1) % 50) + 80;

        if (totalGamesPlayed == 0){

            if(totalStats[0] < 0 || totalStats[0] > 255 || totalStats[1] < 0 || totalStats[1] > 255 || totalStats[2] < 0 || totalStats[2] > 255 || totalStats[3] < 0 || totalStats[3] > 255){
                cout << "Current game stats are out of bounds for a single byte. Cannot write to card." << endl;
                return false;
            } else {

                BYTE currentGameData[4] = {
                    static_cast<BYTE>(totalStats[0]),
                    static_cast<BYTE>(totalStats[1]),
                    static_cast<BYTE>(totalStats[2]),
                    static_cast<BYTE>(totalStats[3])
                };
                
                writeDataToCard(currentGameStartPage, currentGameData, 4, hCardHandle, uActiveProtocol);
                cout << "Wrote first game data to page " << currentGameStartPage << endl;
            }


        } else if (previousGameStartPage > 0){

            // BYTE previousGameData[4];
            // readPage(previousGameStartPage, hCardHandle, uActiveProtocol, previousGameData);

            // cout << "Previous game data read from page " << previousGameStartPage << ": " << (int)previousGameData[0] << ", " << (int)previousGameData[1] << ", " << (int)previousGameData[2] << ", " << (int)previousGameData[3] << endl;

            std::array<int, 4> previousGameData = scardReading::getHistoricTotals(hCardHandle, uActiveProtocol);

            cout << "Current game total stats: " << totalStats[0] << ", " << totalStats[1] << ", " << totalStats[2] << ", " << totalStats[3] << endl;

            BYTE currentKills = totalStats[0] - previousGameData[0];
            BYTE currentDeaths = totalStats[1] - previousGameData[1];
            BYTE currentPrimaryPoints = totalStats[2] - previousGameData[2];
            BYTE currentSecondaryPoints = totalStats[3] - previousGameData[3];

            cout << "Calculated current game stats: " << (int)currentKills << ", " << (int)currentDeaths << ", " << (int)currentPrimaryPoints << ", " << (int)currentSecondaryPoints << endl;

            if(currentKills < 0 || currentKills > 255 || currentDeaths < 0 || currentDeaths > 255 || currentPrimaryPoints < 0 || currentPrimaryPoints > 255 || currentSecondaryPoints < 0 || currentSecondaryPoints > 255){
                cout << "Current game stats are out of bounds for a single byte. Cannot write to card." << endl;
                return false;
            } else {

                BYTE currentGameData[4] = {
                    static_cast<BYTE>(currentKills),
                    static_cast<BYTE>(currentDeaths),
                    static_cast<BYTE>(currentPrimaryPoints),
                    static_cast<BYTE>(currentSecondaryPoints)
                };

                writeDataToCard(currentGameStartPage, currentGameData, 4, hCardHandle, uActiveProtocol);
            }
        }

        totalGamesPlayed++;

        gamesPlayed[0] = (totalGamesPlayed) >> 24;
        gamesPlayed[1] = (totalGamesPlayed) >> 16;
        gamesPlayed[2] = (totalGamesPlayed) >> 8;
        gamesPlayed[3] = (totalGamesPlayed) & 0xFF;

        writeDataToCard(79, gamesPlayed, 4, hCardHandle, uActiveProtocol); //update total games played
        
        return true;
    }
}