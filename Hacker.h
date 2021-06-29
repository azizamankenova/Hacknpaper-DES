#include <string>
using namespace std;

#ifndef PROJECT2_HACKER_H
#define PROJECT2_HACKER_H

class Hacker{
public:
    int id;
    float arrivalTime;
    int numOfCommits;
    int numValidCommits;
    int totalLengthCommits;
    int deskId;
    float waitingTimeSticker;
    float waitingTimeHoodie;
    int numOfGifts;
    float entranceTimeStQ;
    float entranceTimeHoodQ;
    Hacker(float arrivalTime, int id);


};

#endif //PROJECT2_HACKER_H
