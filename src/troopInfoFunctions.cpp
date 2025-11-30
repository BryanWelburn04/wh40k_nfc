#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include "scardHandling.hpp"

using namespace std;

// COMPILE WITH: g++ troopInfoFunctions.cpp -o troopInfoFunc
// THEN RUN: ./troopInfoFunc

struct Troop {
    // Troop game info
    short maxHealth;
    short curHealth;
    short totalKills;
    short totalDeaths;
    
    // Troop ID info
    string troopName;
    short gameID;
    short modelType;
    short troopCount;

    // This is the parameterized constructor
    Troop(const string& name, short maxHP, short curHP,
        short kills, short deaths, short ID, 
        short type, short totalCount) : 
            troopName(name), maxHealth(maxHP),curHealth(curHP), totalKills(kills),
            totalDeaths(deaths), gameID(ID), modelType(type), troopCount(totalCount) {
                cout << "Constructor called for Troop: " << troopName << ", gameID: " << gameID << endl;
            }

    void display() const {
        cout << "Name: " << troopName << ", maxHealth: " << maxHealth << ", curHealth: " << curHealth << endl;
        cout << "gameID: " << gameID << ", troopType: " << modelType << ", troopCount: " << troopCount << endl;
        cout << "Kills: " << totalKills << ", Deaths: " << totalDeaths << endl;
    }
    
    // Troop t1(name, maxHP, curHP, ... , troopCount); calls constructor
    // t1.display() uses the built in display function
};

// This pulls troop info off the SCard and makes a struct for the software
Troop initTroop(BYTE *nameInput, BYTE *infoInput) {
    /* NOTE: may be best to take infoInput and IdInput. This way we can have two arrays of length 4 easy to read from SCard
        Instead of an array of 8 bytes that is a little weird at this point. */
    int nameLength = 52; //sizeof(*nameInput); maybe this works fine too?
    string name;
    // Copy nameInput from array of bytes into a single string and null terminate it
    for (int i = 0; i < nameLength; i++) {
        if (nameInput[i] == 0x00) {
            break; // done reading
        }
        
        char newChar = (char)nameInput[i];
        name.push_back(newChar);
    }
    name.push_back('\0');

    // dgi: [0], type; [1], count: [2], maxHP: [4], curHPL [5] , kills: [6], deaths [7];
    // initialize troop such that curHP == maxHP
    Troop newTroop(name, (short)infoInput[4], (short)infoInput[4], (short)infoInput[6], (short)infoInput[7],
        (short)infoInput[0], (short)infoInput[1], (short)infoInput[2]);

    return newTroop;
}

// Updates SCard and troop struct with new info after taking damage or getting a kill or sum
bool updateTroopGameInfo(Troop *troop, BYTE *infoInput) {
    /* NOTE: Since we have to write 4 bytes at a time we mine as well make this a single function.
        In my head I imagine we will have individual functions to update each specific byte in the input beforehand.
        i.e. updateHealthByte(healthChangeValue, *infoInput);
            updateKillByte(killChangeValue, *infoInput);
            now call updateTroopGameInfo(*troop, *infoInput) after changing some stuff. If nothing changes then dont call. */
    bool success_state = false;
    BYTE curInfo[4] = {(BYTE)troop->maxHealth, (BYTE)troop->curHealth, (BYTE)troop->totalKills, (BYTE)troop->totalDeaths};
    for (int i = 0; i < 4; i++) {
        if (infoInput[i] != curInfo[i]) {
            curInfo[i] = infoInput[i];
        }
    }

    // Pass update info to be written to chip
    // success_state = transmitTroopGameInfo(curInfo);
    success_state = true; // For debug testing
    // If write is success then update struct info to make sure it matches
    //  We can also update each component in the above for loop probably but it may cause de-sync if write fails.
    if (success_state) {
        troop->maxHealth = (short)infoInput[0];
        troop->curHealth = (short)infoInput[1];
        troop->totalKills = (short)infoInput[2];
        troop->totalDeaths = (short)infoInput[3];
    }
    
     return success_state;
}

// Updates Scard and troop struct with new name
bool updateTroopName(Troop *troop, string newName) {
    bool success_state = false;
    int nameLength = newName.length();
    if (nameLength >= 52) {
        cout << "Error: " << newName << " is too long. Need less than 52 characters." << endl;
        return success_state;
    }

    BYTE name[nameLength];
    for (int i = 0; i < nameLength; i++) {
        name[i] = (BYTE)newName[i];
    }

    // success_state = transmitTroopName(name);
    success_state = true; // For debug testing
    // if write success then update struct info to make sure it matches
    if (success_state) {
        troop->troopName = newName;
    }

    return success_state;
}

// Updates Scard and troop struct with new ID info
bool updateTroopIdInfo(Troop *troop, BYTE *idInput) {
    bool success_state = false;
    BYTE curId[4] = {(BYTE)troop->gameID, (BYTE)troop->modelType, (BYTE)troop->troopCount, 0x00}; // 0x00 for placeholder nothing value

    for (int i = 0; i < 4; i++) {
        if (curId[i] != idInput[i]) {
            curId[i] = idInput[i];
        }
    }

    // success_state = transmitTroopIdInfo(curId);
    success_state = true; // For debug testing
    if (success_state) {
        troop->gameID = curId[0];
        troop->modelType = curId[1];
        troop->troopCount = curId[2];
    }
    return success_state;
}

// Initializes an SCard with troop info. Used when writing SCard with troop details for the first time.
bool createSCardTroop(string name, short maxHP, short gameID, short modelType, short troopCount = 1) {
    // can do check for legal name length here or before this function is called.
    // Probably want to check elsewhere beforehand, this way we can reprompt user for valid name
    //  then once its valid, pass into this function?
    bool success_state = false;
    BYTE troopName[52] = {0};
    for (int i = 0; i < name.length(); i++) {
        troopName[i] = (BYTE)name[i];
    }

    BYTE troopGameInfo[4] = {(BYTE)maxHP, (BYTE)maxHP, 0x00, 0x00}; // intialize kills/deaths to 0 for first creation?
    BYTE troopIdInfo[4] = {(BYTE)gameID, (BYTE)modelType, (BYTE)troopCount, 0x00}; // placeholder 0x00

    // success_state = transmitTroopInit(troopName, troopGameInfo, troopIdInfo);
    return success_state;
}


int main() {
    
    // Initializing troop struct test;
    cout << "initTroop() test:" << endl << endl;
    BYTE testName[] = {0x42, 0x72, 0x79, 0x61, 0x6E, 0x00};
    BYTE testInfo[] = {0x0A, 0x02, 0x01, 0xFF, 0x05, 0x03, 0x10, 0x10}; // 0xFF unsused space for now.
    cout << "Name input: " << testName << ", Expected output: Bryan, id: 10" << endl;
    cout << endl;
    cout << "Calling initTroop()..." << endl;
    Troop myTroop = initTroop(testName, testInfo);
    cout << endl;
    cout << "Calling myTroop.display()..." << endl;
    myTroop.display();
    cout << endl;
    cout << "-----------------------" << endl;

    // Testing updateTroopGameInfo()
    cout << "updateTroopGameInfo() test:" << endl << endl;
    BYTE testInfo2[] = {0x06, 0x04, 0x11, 0x11}; // troopInfo 4 bytes from testInfo but increased by 1
    cout << "Initial values after calling myTroop.display()..." << endl;
    myTroop.display();
    cout << endl;
    cout << "Calling updateTroopGameInfo..." << endl;
    bool state = updateTroopGameInfo(&myTroop, testInfo2);
    cout << "New values after update, calling myTroop.display()..." << endl;
    myTroop.display();
    cout << endl;
    cout << "-----------------------" << endl;

    // Testing updateTroopIDInfo()
    cout << "updateTroopIdInfo() test:" << endl;
    BYTE idInfo[] = {0x0B, 0x03, 0x02, 0xFF};
    cout << "Initial values after calling myTroop.display()..." << endl;
    myTroop.display();
    cout << endl;
    cout << "Calling updateTroopIdInfo..." << endl;
    state = updateTroopIdInfo(&myTroop, idInfo);
    cout << "New values after update, calling myTroop.display()..." << endl;
    myTroop.display();
    cout << endl;
    cout << "-----------------------" << endl;

    // Testing updateTroopName()
    cout << "updateTroopName() test:" << endl;
    //BYTE nameInfo[] {0x4d, 0x61, 0x73, 0x6f, 0x6e};
    string nameInfo = "Mason";
    cout << "Initial values after calling myTroop.display()..." << endl;
    myTroop.display();
    cout << endl;
    cout << "Calling updateTroopName..." << endl;
    state = updateTroopName(&myTroop, nameInfo);
    cout << "New values after update, calling myTroop.display()..." << endl;
    myTroop.display();
    cout << endl;
}