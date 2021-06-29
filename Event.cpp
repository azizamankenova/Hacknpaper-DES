#include "Event.h"

using namespace std;

Event::Event(string type, float time, int hackerID){
    this->type = type;
    this->time = time;
    this->hackerID = hackerID;

}
Event::Event(string type, float time, int hackerID, int commitlength){
    this->type = type;
    this->time = time;
    this->hackerID = hackerID;
    this->commitLength = commitlength;
}

