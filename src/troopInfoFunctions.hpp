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
    string greatestAchievement;
    string worstAchievement;
    string link;

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
    short epicHeroKills1;
    short epicHeroKills2;

    short characterKills1;
    short characterKills2;

    //page 8
    short vehicleKills1;
    short vehicleKills2;

    short monsterKills1;
    short monsterKills2;

    //page 9
    short battleLineKills1;
    short battleLineKills2;

    short mountedKills1;
    short mountedKills2;

    //page 10
    short transportKills1;
    short transportKills2;

    short otherKills1;
    short otherKills2;


    //page 11
    short pointCost1;
    short pointCost2;
    short PHDS1;
    short PHDS2;

    //page 12
    short PHDS3;
    short PHDS4;
    short PHDS5;
    short PHDS6;





    // This is the parameterized constructor
    Troop(
        const string& name,
        short kills1, short kills2, short deaths1, short deaths2,
        short primaryPts1, short primaryPts2, short secondaryPts1, short secondaryPts2,
        short maxHP, short curHP, short type, short totalCount,
        short epicHeroKills1, short epicHeroKills2, short characterKills1, short characterKills2,
        short vehicleKills1, short vehicleKills2, short monsterKills1, short monsterKills2,
        short battleLineKills1, short battleLineKills2, short mountedKills1, short mountedKills2,
        short transportKills1, short transportKills2, short otherKills1, short otherKills2,
        short pointCost1, short pointCost2, short PHDS1, short PHDS2,
        const string& greatestAchievement,
        const string& worstAchievement,
        const string& link
    ) : 
        troopName(name),
        totalKills1(kills1), totalKills2(kills2), totalDeaths1(deaths1), totalDeaths2(deaths2),
        primaryPoints1(primaryPts1), primaryPoints2(primaryPts2), secondaryPoints1(secondaryPts1), secondaryPoints2(secondaryPts2),
        maxHealth(maxHP), curHealth(curHP), modelType(type), troopCount(totalCount),
        epicHeroKills1(epicHeroKills1), epicHeroKills2(epicHeroKills2), characterKills1(characterKills1), characterKills2(characterKills2),
        vehicleKills1(vehicleKills1), vehicleKills2(vehicleKills2), monsterKills1(monsterKills1), monsterKills2(monsterKills2),
        battleLineKills1(battleLineKills1), battleLineKills2(battleLineKills2), mountedKills1(mountedKills1), mountedKills2(mountedKills2),
        transportKills1(transportKills1), transportKills2(transportKills2), otherKills1(otherKills1), otherKills2(otherKills2),
        pointCost1(pointCost1), pointCost2(pointCost2), PHDS1(PHDS1), PHDS2(PHDS2),
        greatestAchievement(greatestAchievement),
        worstAchievement(worstAchievement),
        link(link)
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

Troop initTroop(BYTE *nameInput, BYTE *infoInput, BYTE *gaInput, BYTE *waInput, BYTE *linkInput);
void updateStructInfo(Troop *troop, BYTE *infoInput);
void updateTroopName(Troop *troop, string newName);
void updateTroopGA(Troop *troop, string newName);
void updateTroopWA(Troop *troop, string newName);
void updateTroopLink(Troop *troop, string newName);


#endif