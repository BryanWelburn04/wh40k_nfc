#include "dataTransferAlgorithms.hpp"
#include "troopInfoFunctions.hpp"
#include "constants.hpp"

namespace dataTransferAlgorithms {

    void swapTroopData(Troop& troopA, int troopANewHealth, Troop& troopB, int troopBNewHealth){

        int troopADamageTaken = troopA->curHealth - troopANewHealth;
        int troopBDamageTaken = troopB->curHealth - troopBNewHealth;

        int troopAHelthPerModel = troopAMaxHealth/troopA->numModels;
        int troopBHelthPerModel = troopBMaxHealth/troopB->numModels;

        int troopADeaths = troopADamageTaken / troopAHelthPerModel;
        int troopBDeaths = troopBDamageTaken / troopBHelthPerModel;

        int troopATotalKills = troopA->totalKills + troopBDeaths;
        int troopBTotalKills = troopB->totalKills + troopADeaths;

        int troopATotalDeaths = troopA->totalDeaths + troopADeaths;
        int troopBTotalDeaths = troopB->totalDeaths + troopBDeaths;

        int troopAKDRatio = troopATotalKills / (troopATotalDeaths > 0 ? troopATotalDeaths : 1);
        int troopBKDRatio = troopBTotalKills / (troopBTotalDeaths > 0 ? troopBTotalDeaths : 1);

        int troopAGamesPlayed = ... + 1;
        int troopBGamesPlayed = ... + 1;

        float troopAPointsLostInHealth = troopADamageTaken * (troopA->pointCost / troopA->maxHealth);
        float troopBPointsLostInHealth = troopBDamageTaken * (troopB->pointCost / troopB->maxHealth);

        // int troopAAllTimePointsSpentOnModel = troopAGamesPlayed * troopA->pointCost;
        // int troopBAllTimePointsSpentOnModel = troopBGamesPlayed * troopB->pointCost;


        float troopAPointsPerDamageDeltComparedToOwnPointCostTotalDeficitOrSurplus = troopA->PHDS + troopBPointsLostInHealth;
        float troopBPointsPerDamageDeltComparedToOwnPointCostTotalDeficitOrSurplus = troopB->PHDS + troopAPointsLostInHealth;

        if(troopB->type == EPIC_HERO){
            troopA->epicHeroKills += troopBDeaths;
        } else if (troopB->type == CHARACTER){
            troopA->characterKills += troopBDeaths;
        } else if (troopB->type == VEHICLE){
            troopA->vehicleKills += troopBDeaths;
        } else if (troopB->type == MONSTER){
            troopA->monsterKills += troopBDeaths;
        } else if (troopB->type == BATTLELINE){
            troopA->battlelineKills += troopBDeaths;
        } else if (troopB->type == MOUNTED){
            troopA->mountedKills += troopBDeaths;
        } else if (troopB->type == TRANSPORT){
            troopA->transportKills += troopBDeaths;
        } else if (troopB->type == OTHER){
            troopA->otherKills += troopBDeaths;
        }

        if(troopA->type == EPIC_HERO){
            troopB->epicHeroKills += troopADeaths;
        } else if (troopB->type == CHARACTER){
            troopB->characterKills += troopADeaths;
        } else if (troopB->type == VEHICLE){
            troopB->vehicleKills += troopADeaths;
        } else if (troopB->type == MONSTER){
            troopB->monsterKills += troopADeaths;
        } else if (troopB->type == BATTLELINE){
            troopB->battlelineKills += troopADeaths;
        } else if (troopB->type == MOUNTED){
            troopB->mountedKills += troopADeaths;
        } else if (troopB->type == TRANSPORT){
            troopB->transportKills += troopADeaths;
        } else if (troopB->type == OTHER){
            troopB->otherKills += troopADeaths;
        }

}

}