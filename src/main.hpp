#ifndef MAIN_H
#define MAIN_H

int main();
void waitForCard (std::string selectedReaderName, SCARDCONTEXT smartCardContext);
void waitForCardImproved(std::string selectedReaderName, SCARDCONTEXT smartCardContext);
void setStateForGetStatusChange(SCARDCONTEXT smartCardContext, SCARD_READERSTATE &readerState0);


#endif