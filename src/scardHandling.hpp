#ifndef SCARDHANDLING_H
#define SCARDHANDLING_H

bool establishContext(SCARDCONTEXT& readerList);
void getReaderList(SCARDCONTEXT all_readers, char* &pReaderString);
bool connectToReader(std::string readerName, SCARDCONTEXT smartCardContext);
void getCardDetails(SCARDCONTEXT smartCardContext, std::string selectedReaderName);

#endif