#include "NFCReader.hpp"
#include <iostream>
#include "scardHandling.hpp"
#include "scardReading.hpp"
#include "scardWriting.hpp"

NFCReader::NFCReader(const std::wstring& name)
    : readerName(name)
{
}



/* =========================================== */
/* ========= SCARD HANDLING FUNCTIONS ======== */
/* =========================================== */

void NFCReader::initializeReader() {
 //   std::wcout << L"Before Initializing reader: " << readerName << L" with handle: " << hCardHandle << L" and protocol: " << activeProtocol << L"state: " << readerState.dwCurrentState << std::endl;
    scardHandling::initializeReader(readerName.c_str(), smartCardContext, readerState);
  //  std::wcout << L"After Initializing reader: " << readerName << L" with handle: " << hCardHandle << L" and protocol: " << activeProtocol << L"state: " << readerState.dwCurrentState << std::endl;
}

bool NFCReader::waitForCard() {
    return scardHandling::waitForCard(readerName.c_str(), smartCardContext, readerState, hCardHandle, activeProtocol);
}

bool NFCReader::waitForCardRemoval() {
    return scardHandling::waitForCardRemoval(smartCardContext, readerState, hCardHandle);
}


/* =========================================== */
/* ========= SCARD READING FUNCTIONS ========= */
/* =========================================== */

bool NFCReader::readPage(unsigned char startPage, BYTE *infoContainer){
    return scardReading::readPage(startPage, hCardHandle, activeProtocol, infoContainer);
}

bool NFCReader::readPages(unsigned char startPage, unsigned char endPage, BYTE *infoContainer){
    return scardReading::readPages(startPage, endPage, hCardHandle, activeProtocol, infoContainer);
}

QString NFCReader::getRawDataFromCard(BYTE *cardData, unsigned char startPage, unsigned char endPage) {
    return scardReading::getRawDataFromCard(cardData, startPage, endPage);
}


/* =========================================== */
/* ========= SCARD WRITING FUNCTIONS ========= */
/* =========================================== */

void NFCReader::writeDataToCard(unsigned char startPage, BYTE *dataForCard, size_t sizeOfData){
    scardWriting::writeDataToCard(startPage, dataForCard, sizeOfData, hCardHandle, activeProtocol);
}

void NFCReader::resetPageData(unsigned char startPage, unsigned char endPage){
    scardWriting::resetPageData(startPage, endPage, hCardHandle, activeProtocol);

}
bool NFCReader::writeStatsToCard(BYTE *dataForCard, size_t sizeOfData){
    return scardWriting::writeStatsToCard(dataForCard, sizeOfData, hCardHandle, activeProtocol);
}

bool NFCReader::writeNameToCard(BYTE *dataForCard, size_t sizeOfData){
    return scardWriting::writeNameToCard(dataForCard, sizeOfData, hCardHandle, activeProtocol);
}

bool NFCReader::writeLinkToCard(BYTE *dataForCard, size_t sizeOfData){
    return scardWriting::writeLinkToCard(dataForCard, sizeOfData, hCardHandle, activeProtocol);
}

bool NFCReader::writeGreatestAchievementToCard(BYTE *dataForCard, size_t sizeOfData){
    return scardWriting::writeGreatestAchievementToCard(dataForCard, sizeOfData, hCardHandle, activeProtocol);
}

bool NFCReader::writeWorstAchievementToCard(BYTE *dataForCard, size_t sizeOfData){
    return scardWriting::writeWorstAchievementToCard(dataForCard, sizeOfData, hCardHandle, activeProtocol);
}

bool NFCReader::writeHistoryToCard(int *totalStats, size_t sizeOfData){
    return scardWriting::writeHistoryToCard(totalStats, sizeOfData, hCardHandle, activeProtocol);
}

//other
const std::wstring& NFCReader::getName() const
{
    return readerName;
}

void NFCReader::printInfo() {
    std::wcout << L"Reader Name: " << readerName << std::endl;
    std::wcout << L"Card Handle: " << hCardHandle << std::endl;
    std::wcout << L"Active Protocol: " << activeProtocol << std::endl;
    std::wcout << L"Reader State: " << readerState.dwCurrentState << std::endl;
}