#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include "scardHandling.hpp"
#include "troopInfoFunctions.hpp"


// COMPILE WITH: g++ troopInfoFunctions.cpp -o troopInfoFunc
// THEN RUN: ./troopInfoFunc


// This pulls troop info off the SCard and makes a struct for the software
Troop initTroop(BYTE *nameInput, BYTE *infoInput, BYTE *gaInput, BYTE *waInput, BYTE *linkInput) {

    int nameLength = 52;
    std::string name;

    for (int i = 0; i < nameLength; i++) {
        if (nameInput[i] == 0x00) break;
        name.push_back((char)nameInput[i]);
    }

    short totalKills1      = (short)infoInput[0];
    short totalKills2      = (short)infoInput[1];
    short totalDeaths1     = (short)infoInput[2];
    short totalDeaths2     = (short)infoInput[3];
    short primaryPoints1   = (short)infoInput[4];
    short primaryPoints2   = (short)infoInput[5];
    short secondaryPoints1 = (short)infoInput[6];
    short secondaryPoints2 = (short)infoInput[7];
    short maxHealth        = (short)infoInput[8];
    short curHealth        = (short)infoInput[9];
    short modelType        = (short)infoInput[10];
    short troopCount       = (short)infoInput[11];
    short epicHeroKills1    = (short)infoInput[12];
    short epicHeroKills2    = (short)infoInput[13];
    short characterKills1   = (short)infoInput[14];
    short characterKills2   = (short)infoInput[15];
    short vehicleKills1     = (short)infoInput[16];
    short vehicleKills2     = (short)infoInput[17];
    short monsterKills1     = (short)infoInput[18];
    short monsterKills2     = (short)infoInput[19];
    short battleLineKills1  = (short)infoInput[20];
    short battleLineKills2  = (short)infoInput[21];
    short mountedKills1     = (short)infoInput[22];
    short mountedKills2     = (short)infoInput[23];
    short transportKills1   = (short)infoInput[24];
    short transportKills2   = (short)infoInput[25];
    short otherKills1       = (short)infoInput[26];
    short otherKills2       = (short)infoInput[27];
    short pointCost1       = (short)infoInput[28];
    short pointCost2       = (short)infoInput[29];
    short PHDS1            = (short)infoInput[30];
    short PHDS2            = (short)infoInput[31];



    Troop newTroop(
        name,
        totalKills1,
        totalKills2,
        totalDeaths1,
        totalDeaths2,
        primaryPoints1,
        primaryPoints2,
        secondaryPoints1,
        secondaryPoints2,
        maxHealth,
        curHealth,
        modelType,
        troopCount,
        epicHeroKills1,
        epicHeroKills2,
        characterKills1,
        characterKills2,
        vehicleKills1,
        vehicleKills2,
        monsterKills1,
        monsterKills2,
        battleLineKills1,
        battleLineKills2,
        mountedKills1,
        mountedKills2,
        transportKills1,
        transportKills2,
        otherKills1,
        otherKills2,
        pointCost1,
        pointCost2,
        PHDS1,
        PHDS2,
        std::string((char *)gaInput),
        std::string((char *)waInput),
        std::string((char *)linkInput)
    );

    return newTroop;
}

void updateStructInfo(Troop *troop, BYTE *infoInput){
    troop->totalKills1      = (short)infoInput[0];
    troop->totalKills2      = (short)infoInput[1];
    troop->totalDeaths1     = (short)infoInput[2];
    troop->totalDeaths2     = (short)infoInput[3];
    troop->primaryPoints1   = (short)infoInput[4];
    troop->primaryPoints2   = (short)infoInput[5];
    troop->secondaryPoints1 = (short)infoInput[6];
    troop->secondaryPoints2 = (short)infoInput[7];
    troop->maxHealth        = (short)infoInput[8];
    troop->curHealth        = (short)infoInput[9];
    troop->modelType        = (short)infoInput[10];
    troop->troopCount       = (short)infoInput[11];
    troop->epicHeroKills1    = (short)infoInput[12];
    troop->epicHeroKills2    = (short)infoInput[13];
    troop->characterKills1   = (short)infoInput[14];
    troop->characterKills2   = (short)infoInput[15];
    troop->vehicleKills1     = (short)infoInput[16];
    troop->vehicleKills2     = (short)infoInput[17];
    troop->monsterKills1     = (short)infoInput[18];
    troop->monsterKills2     = (short)infoInput[19];
    troop->battleLineKills1  = (short)infoInput[20];
    troop->battleLineKills2  = (short)infoInput[21];
    troop->mountedKills1     = (short)infoInput[22];
    troop->mountedKills2     = (short)infoInput[23];
    troop->transportKills1   = (short)infoInput[24];
    troop->transportKills2   = (short)infoInput[25];
    troop->otherKills1       = (short)infoInput[26];
    troop->otherKills2       = (short)infoInput[27];
    troop->pointCost1       = (short)infoInput[28];
    troop->pointCost2       = (short)infoInput[29];
    troop->PHDS1            = (short)infoInput[30];
    troop->PHDS2            = (short)infoInput[31];
    return;
}


/*

// update kills, deaths, primary, secondary
// packet 1 will be recoded in history
void updatePacket1(Troop *troop, BYTE *infoInput){
    troop->totalKills = (short)infoInput[0];
    troop->totalDeaths = (short)infoInput[1];
    troop->primaryPoints = (short)infoInput[2];
    troop->secondaryPoints = (short)infoInput[3];
    return;
}

// update health, model type, troop count
void updatePacket2(Troop *troop, BYTE *infoInput) {
    troop->maxHealth = (short)infoInput[0];
    troop->curHealth = (short)infoInput[1];
    troop->modelType = (short)infoInput[2];
    troop->troopCount = (short)infoInput[3];
    return;
}

*/

void updateTroopName(Troop *troop, string newName) {

    int nameLength = int(newName.length());
    if (nameLength >= 52) {
        cout << "Error: " << newName << " is too long. Need less than 52 characters." << endl;
        return;
    }

    BYTE name[52];
    for (int i = 0; i < nameLength; i++) {
        name[i] = (BYTE)newName[i];
    }

    troop->troopName = newName;

    return;
}

void updateTroopGA(Troop *troop, string greatestAchievement) {

    int nameLength = int(greatestAchievement.length());
    if (nameLength >= 40) {
        cout << "Error: " << greatestAchievement << " is too long. Need less than 40 characters." << endl;
        return;
    }

    BYTE name[40];
    for (int i = 0; i < nameLength; i++) {
        name[i] = (BYTE)greatestAchievement[i];
    }

    troop->greatestAchievement = greatestAchievement;

    return;
}

void updateTroopWA(Troop *troop, string worstAchievement) {

    int nameLength = int(worstAchievement.length());
    if (nameLength >= 40) {
        cout << "Error: " << worstAchievement << " is too long. Need less than 40 characters." << endl;
        return;
    }

    BYTE name[40];
    for (int i = 0; i < nameLength; i++) {
        name[i] = (BYTE)worstAchievement[i];
    }

    troop->worstAchievement = worstAchievement;

    return;
}

void updateTroopLink(Troop *troop, string link) {

    int nameLength = int(link.length());
    if (nameLength >= 148) {
        cout << "Error: " << link << " is too long. Need less than 148 characters." << endl;
        return;
    }

    BYTE name[148];
    for (int i = 0; i < nameLength; i++) {
        name[i] = (BYTE)link[i];
    }

    troop->worstAchievement = link;

    return;
}






















// =============== OLD FUNCTIONS FOR REFRENCE ===============

/*

// Updates SCard and troop struct with new info after taking damage or getting a kill or sum
bool updateTroopGameInfo(Troop *troop, BYTE *infoInput) {
    //  NOTE: Since we have to write 4 bytes at a time we mine as well make this a single function.
    //     In my head I imagine we will have individual functions to update each specific byte in the input beforehand.
    //     i.e. updateHealthByte(healthChangeValue, *infoInput);
    //         updateKillByte(killChangeValue, *infoInput);
    //         now call updateTroopGameInfo(*troop, *infoInput) after changing some stuff. If nothing changes then dont call. 
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
    int nameLength = int(newName.length());
    if (nameLength >= 52) {
        cout << "Error: " << newName << " is too long. Need less than 52 characters." << endl;
        return success_state;
    }

    BYTE name[52];
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

*/