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

    //page 4
    short totalKills1;
    short totalKills2;
    short totalDeaths1;
    short totalDeaths2;

    //page 5
    short primaryPoints1;
    short primaryPoints2;
    short secondaryPoints1;
    short secondaryPoints2;

    //page 6
    short maxHealth;
    short curHealth;
    short modelType;
    short troopCount;

    //page 7
    short epicHeroKills;
    short characterKills;
    short vehicleKills;
    short monsterKills;

    //page 8
    short battleLineKills;
    short mountedKills;
    short transportKills;
    short otherKills;

    //page 9
    short pointCost1;
    short pointCost2;
    short PHDS1;
    short PHDS2;



    // This is the parameterized constructor
    Troop(
        const string& name,
        short kills1, short kills2, short deaths1, short deaths2,
        short primaryPts1, short primaryPts2, short secondaryPts1, short secondaryPts2,
        short maxHP, short curHP, short type, short totalCount,
        short epicHeroKills, short characterKills, short vehicleKills, short monsterKills,
        short battleLineKills, short mountedKills, short transportKills, short otherKills,
        short pointCost1, short pointCost2, short PHDS1, short PHDS2
    ) : 
        troopName(name),
        totalKills1(kills1), totalKills2(kills2), totalDeaths1(deaths1), totalDeaths2(deaths2),
        primaryPoints1(primaryPts1), primaryPoints2(primaryPts2), secondaryPoints1(secondaryPts1), secondaryPoints2(secondaryPts2),
        maxHealth(maxHP), curHealth(curHP), modelType(type), troopCount(totalCount),
        epicHeroKills(epicHeroKills), characterKills(characterKills), vehicleKills(vehicleKills), monsterKills(monsterKills),
        battleLineKills(battleLineKills), mountedKills(mountedKills), transportKills(transportKills), otherKills(otherKills),
        pointCost1(pointCost1), pointCost2(pointCost2), PHDS1(PHDS1), PHDS2(PHDS2)
        {
        
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
void updateStructInfo(Troop *troop, BYTE *infoInput);
void updateTroopName(Troop *troop, string newName);

#endif