#ifndef TROOPINFOFUNCTIONS_H
#define TROOPINFOFUNCTIONS_H

#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>

using namespace std;

struct Troop {

    string troopName;

    // Troop game info (data pack #1)
    short totalKills;
    short totalDeaths;
    short primaryPoints;
    short secondaryPoints;
    
    // Troop ID info (data pack #2)
    short maxHealth;
    short curHealth;
    short modelType;
    short troopCount;


    // This is the parameterized constructor
    Troop(const string& name,
        short kills, short deaths, short primaryPts, short secondaryPts,
        short maxHP, short curHP, short type, short totalCount) : 
            troopName(name),
            totalKills(kills), totalDeaths(deaths), primaryPoints(primaryPts), secondaryPoints(secondaryPts),
            maxHealth(maxHP), curHealth(curHP), modelType(type), troopCount(totalCount) {
                cout << "Constructor called for Troop: " << troopName << endl;
            }

    // need to update this to include new info
/*
    void display() const {
        cout << "Name: " << troopName << ", maxHealth: " << maxHealth << ", curHealth: " << curHealth << endl;
        cout << "troopType: " << modelType << ", troopCount: " << troopCount << endl;
        cout << "Kills: " << totalKills << ", Deaths: " << totalDeaths << endl;
    }
*/
    
    // Troop t1(name, maxHP, curHP, ... , troopCount); calls constructor
    // t1.display() uses the built in display function
};

Troop initTroop(BYTE *nameInput, BYTE *infoInput);
void updatePacket1(Troop *troop, BYTE *infoInput);
void updatePacket2(Troop *troop, BYTE *idInput);
void updateTroopName(Troop *troop, string newName);

#endif