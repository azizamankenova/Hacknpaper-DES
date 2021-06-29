#include "Hacker.h"


Hacker::Hacker(float arrivalTime,int id){
    this->arrivalTime = arrivalTime;
    this->id = id;
    this->numOfCommits = 0;
    this->numValidCommits = 0;
    this->totalLengthCommits = 0;
    this->waitingTimeSticker = 0;
    this->waitingTimeHoodie = 0;
    this->numOfGifts = 0;
    this->entranceTimeStQ = 0;
    this->entranceTimeHoodQ = 0;
    this->deskId = 0;

}


