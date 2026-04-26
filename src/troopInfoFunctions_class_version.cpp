#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <winscard.h>
#include "scardHandling.hpp"
#include "troopInfoFunctions.hpp"

#include <list>

using namespace std;

/* EXPLANATION FOR CLASS DATA STRUCTURE:
 * Still not entirely sure on the main reasons we should use classes vs. structs. 
 * As far as I can tell, this class structure seems to be a lot easier to understand. 
 * It allows me to define functionality for handling each member or class object in one place.
 * The main benefit I can see immediately is that we do not need to pass or return our Troop struct references. 
 * Instead we can simply access the troop object and use an associated function to do what we need. */

/* WHERE TO FROM HERE? 
 * Im still trying to decide what functionalities are necessary.
 * I have implemented individual member variable control alongside being able to pass all pages directly from Reader in byte arrays.
 * I dont believe functions to write data into the SCard should be in this class though. 
 * Mainly I think this class is for the main program side of things i.e. managing and displaying this information for the user. */

/* CONCERNS && CONSIDERATIONS 
 * 1) if user has a formatted troop SCARD and tries to intialize them as a new troop into the program but they already exist in the troop JSON file.
 * If this occurs, We must prevent the creation of a new troop and instead update the existing troop object to the scanned SCARD information. 
 *
 * 2) This class has Creation and Updating functions defined, the compiler will create a Deletion function that simply deletes everything which is OK
 * 
 * 3) We should use JSON to store initilalized troop objects to the program so troops do not have to be initialized every session.
 * 
 * 4) Troop deletion could be tricky, if we create troops with a linear ID number, in the event we delete a troop it will leave an empty index slot.
 * This could lead to us running out of indexes after enough deletions. We must find a way to remedy this. 
 * One solution could be to use a hashmap 
 * 
 * 5) We can enfore member variable limits inside of these constrcutor and setter functions. This will allow us to make sure values dont overflow.
 * Mainly im thinking about what happens if a troop dies 255 times, once more and it becomes 0, How should we handle this situation? */

// START NAMING CONVENTION:
// Functions that take BYTE type parameters will be named with _B at the end i.e exampleFunction_B(BYTE num_b);
// Variables that consist of BYTE type will be named with _b at the end i.e BYTE exampleVariable_b;

class Troop {
    // memember names use C++ member naming convention --> this is why underscore
    short maxHealth_;
    short curHealth_;
    short totalKills_;
    short totalDeaths_;

    string troopName_;
    short gameID_;
    short modelType_;
    short troopCount_;

    public:
        // Maybe make two constructors: short type params vs. BYTE type params.
        Troop(short maxHP, short curHP, short kills, short deaths,
            string name, short ID, short type, short count) {
                maxHealth_ = maxHP;
                curHealth_ = curHP;
                totalKills_ = kills;
                totalDeaths_ = deaths;
                troopName_ = name;
                gameID_ = ID;
                modelType_ = type;
                troopCount_ = count;
        }

        // Basic getter functions for use in regular user applications
        short getMaxHealth() { return maxHealth_; }
        short getCurHealth() { return curHealth_; }
        short getTotalKills() { return totalKills_; }
        short getTotalDeaths() { return totalDeaths_; }
        string getTroopName() { return troopName_; }
        short getGameID() { return gameID_; }
        short getModelType() { return modelType_; }
        short getTroopCount() { return troopCount_; }

        // getter functions for byte type, to be used when sending information to SCard
        BYTE getMaxHealth_B() { return (BYTE)maxHealth_; }
        BYTE getCurHealth_B() { return (BYTE)curHealth_; }
        BYTE getTotalKills_B() { return (BYTE)totalKills_; }
        BYTE getTotalDeaths_B() { return (BYTE)totalDeaths_; }
        BYTE getGameID_B() { return (BYTE)gameID_; }
        BYTE getModelType_B() { return (BYTE)modelType_; }
        BYTE getTroopCount_B() { return (BYTE)troopCount_; }
        list<BYTE> getTroopName_B() { // Unsure why but return type needs to be pointer cannot return a regular byte array by value
            short nameLength = troopName_.length();
            list<BYTE> name_b;
            for (int i = 0; i < nameLength; i++) {
                name_b.push_back((BYTE)troopName_[i]);
            }
            return name_b;
        }
        
        // Basic setter functions, typecast so can be used directly from SCard info and regular short inputs
        // Tested this in WSL2, C++ does not care if we pass BYTE (unsigned char) in place of a short. It just converts and it works.
        //      we should test this in this version of C++ we are using just in case though.
        void setMaxHealth(short num) { maxHealth_ = num; }
        void setCurHealth(short num) { curHealth_ = num; }
        void setTotalKills(short num) { totalKills_ = num; }
        void setTotalDeaths(short num) { totalDeaths_ = num; }
        void setTroopName(string name) { troopName_ = name; }
        void setGameID(short num) { gameID_ = num; }
        void setModelType(short num) { modelType_ = num; }
        void setTroopCount(short num) { troopCount_ = num; }

        // Display function
        void display() const {
            cout << "Name: " << troopName_ << ", maxHealth: " << maxHealth_ << ", curHealth: " << curHealth_ << endl;
            cout << "gameID: " << gameID_ << ", troopType: " << modelType_ << ", troopCount: " << troopCount_ << endl;
            cout << "Kills: " << totalKills_ << ", Deaths: " << totalDeaths_ << endl;
        }

        void setTroopInfo_B(BYTE *info_b, BYTE *idInfo_b, BYTE *name_b) {
            // info && idInfo both will be 4 bytes in length
            // name can be up to 52 bytes in length.
            setMaxHealth(info_b[0]);
            setCurHealth(info_b[1]);
            setTotalKills(info_b[2]);
            setTotalDeaths(info_b[3]);
            // NOTE: All of these inputs MAY have to be typecasted as a short.
            setGameID(idInfo_b[0]);
            setModelType(idInfo_b[1]);
            setTroopCount(idInfo_b[2]);

            string name;
            for (int i = 0; i < 52; i++) {
                name.push_back((char)name_b[i]); // name += (char)name_b[i]; would also work I think.
            }
            setTroopName(name);
        }

        list<BYTE> getTroopInfo_B() {
            // Return by value
            list<BYTE> info;
            info.push_back((BYTE)maxHealth_);
            info.push_back((BYTE)curHealth_);
            info.push_back((BYTE)totalKills_);
            info.push_back((BYTE)totalDeaths_);
            return info;
        }

        list<BYTE> getTroopIdInfo_B() {
            // Return by value
            list<BYTE> info;
            info.push_back((BYTE)gameID_);
            info.push_back((BYTE)modelType_);
            info.push_back((BYTE)troopCount_);
            info.push_back(0x00);
            return info;
        }
};

int main() {

    cout << "Begin Class Testing." << endl;
    cout << endl;
    cout << "Using troop constructor and display method." << endl;
    Troop bryan(16, 16, 2, 3, "Bryan", 1, 13, 1);
    bryan.display();
    cout << endl << endl;

    cout << "Testing getTroopName_B()" << endl;
    cout << "Expecting: B r y a n" << endl;
    list<BYTE> name_b = bryan.getTroopName_B();
    cout << "Output: ";
    for (BYTE b : name_b) {
        cout << b << " ";
    }
    cout << endl << endl;
    return 0;
}