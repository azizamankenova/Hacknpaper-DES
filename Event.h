#include <string>
#include <vector>
#include "Hacker.h"

using namespace std;

#ifndef PROJECT2_EVENT_H
#define PROJECT2_EVENT_H

class Event{
public:
    string type;
    float time;
    int hackerID;
    int commitLength;
    Event(string type, float time, int hackerID);
    Event(string type, float time, int hackerID, int commitlength);
};


#endif //PROJECT2_EVENT_H
