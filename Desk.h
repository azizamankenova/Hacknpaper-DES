#include <string>
using namespace std;

#ifndef PROJECT2_DESK_H
#define PROJECT2_DESK_H

class Desk{
public:
    int id;
    bool isAvailable;
    string type;//hoodie or sticker
    float serviceTime;
    Desk(int id, float serviceTime, string type);
};

#endif //PROJECT2_DESK_H
