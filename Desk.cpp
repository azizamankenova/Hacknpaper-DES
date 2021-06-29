#include "Desk.h"

Desk::Desk(int id, float serviceTime, string type) {
    this->id = id;
    this->serviceTime = serviceTime;
    this->type = type;
    this->isAvailable = true;
}

