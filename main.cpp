#include <iostream>
#include <fstream>
#include <vector>
#include "Hacker.h"
#include <queue>
#include "Event.h"
#include "Desk.h"
#include <iomanip>
#include <chrono>
using namespace std;
struct comp {
    bool operator()(const Event &e1, const Event &e2) {
        if (abs(e1.time - e2.time) < 0.00001) {
            return e1.hackerID > e2.hackerID;
        } else {
            return e1.time - e2.time > 0.00001;
        }
    }
};
struct compare {
    bool operator()(const Hacker* h1, const Hacker* h2) {
        if (h1->numValidCommits == h2->numValidCommits) {
            if (abs(h1->entranceTimeHoodQ - h2->entranceTimeHoodQ) < 0.00001) {
                return h1->id > h2->id;
            } else {
                return h1->entranceTimeHoodQ - h2->entranceTimeHoodQ > 0.00001;
            }
        } else {
            return h1->numValidCommits < h2->numValidCommits;
        }
    }
};


    int main(int argc, char* argv[]) {

    ifstream infile;
    infile.open(argv[1]);
    ofstream outfile;
    outfile.open(argv[2]);
    int numOfHackers;
    infile >> numOfHackers;
    vector<Hacker> hackersList;
    vector<Desk> stickerDesks;
    vector<Desk> hoodieDesks;
    priority_queue<Event, vector<Event>, comp> heapOfEvents;
    for (int i = 0; i < numOfHackers; i++) {
        float arrivalTime;
        infile >> arrivalTime;
        hackersList.emplace_back(Hacker(arrivalTime, i + 1));
        //heapOfEvents.push(Event("Arrival", arrivalTime, i + 1));
    }

    float numCodeCommits;
    infile >> numCodeCommits;
    for (int i = 0; i < numCodeCommits; i++) {
        int id, length;
        float time;
        infile >> id >> length >> time;
        heapOfEvents.push(Event("CodeCommit", time, id, length));
    }
    int numQueueEntrances;
    infile >> numQueueEntrances;
    for (int i = 0; i < numQueueEntrances; i++) {
        int id;
        float time;
        infile >> id >> time;
        heapOfEvents.push((Event("StQueueEntrance", time, id)));
    }
    int numStickDesks;
    infile >> numStickDesks;
    for (int i = 0; i < numStickDesks; i++) {
        float serviceTime;
        infile >> serviceTime;
        stickerDesks.emplace_back((Desk(i, serviceTime, "Sticker")));

    }
    int numHoodieDesks;
    infile >> numHoodieDesks;
    for(int i = 0; i < numHoodieDesks; i++){
        float serviceTime;
        infile >> serviceTime;
        hoodieDesks.emplace_back((Desk(i, serviceTime, "Hoodie")));
    }
    queue<Hacker*> stickerQueue;
    priority_queue<Hacker*, vector<Hacker*>, compare> hoodieQueue;

    int countInvAtStQ = 0;
    int countInvAttEnoughGifts = 0;
    int maxSizeHoodieQ = 0, maxSizeStickerQ = 0;
    float time = 0;
    float waitingTimeStQ = 0;
    float waitingTimeHoQ = 0;
    float turnAroundTimes = 0;

    while(!heapOfEvents.empty()){
        Event event = heapOfEvents.top();
        time = event.time;
        heapOfEvents.pop();

        if(event.type == "CodeCommit"){
            Hacker &hacker = hackersList[event.hackerID - 1];
            hacker.numOfCommits++;
            if(event.commitLength >= 20) {
                hacker.numValidCommits++;
            }
            hacker.totalLengthCommits += event.commitLength;
        }
        else if(event.type == "StQueueEntrance"){
            Hacker &hacker = hackersList[event.hackerID - 1];
            if(hacker.numValidCommits < 3 ){
                countInvAtStQ++;
            }
            else if(hacker.numOfGifts == 3){
                countInvAttEnoughGifts++;
            }
            else{
                int i;
                for(i = 0; i < stickerDesks.size(); i++){
                    //if some desk is available send the hacker there
                    Desk &stdesk = stickerDesks[i];
                    if(stdesk.isAvailable) {
                        stdesk.isAvailable = false;
                        hacker.deskId = i;
                        hacker.entranceTimeStQ = time;
                        heapOfEvents.push(Event("LeaveStDesk", time + stdesk.serviceTime, hacker.id));
                        
                        break;
                    }
                }
                //no desks are available so go to the sticker queue
                if(i >= stickerDesks.size()){
                    hacker.entranceTimeStQ = time;
                    stickerQueue.push(&hacker);
                    if(maxSizeStickerQ < stickerQueue.size()){
                        maxSizeStickerQ = stickerQueue.size();
                    }
                }
            }
        }
        else if(event.type == "LeaveStDesk"){
            Hacker &hacker = hackersList[event.hackerID - 1];
            Desk &stickDesk = stickerDesks[hacker.deskId];
            stickerDesks[hacker.deskId].isAvailable = true;
            int i;
            //if there is an available desk, hacker goes into the hoodie desk
            for(i = 0; i < hoodieDesks.size(); i++){
                Desk &hoodDesk = hoodieDesks[i];
                if(hoodDesk.isAvailable){
                    hacker.deskId = i;
                    hoodDesk.isAvailable = false;
                    hacker.entranceTimeHoodQ = time;
                    heapOfEvents.push(Event("LeaveHoodDesk", time + hoodDesk.serviceTime, hacker.id));

                    break;
                }
            }
            //if there is no available desk, hacker goes into hoodie queue
            if(i >= hoodieDesks.size()){
                hacker.entranceTimeHoodQ = time;
                hoodieQueue.push(&hacker);
                if(maxSizeHoodieQ < hoodieQueue.size()){
                    maxSizeHoodieQ = hoodieQueue.size();
                }
            }

            //moving hacker from st queue into the avaliable desk
            if(!stickerQueue.empty()){
                Hacker* hacker2 = stickerQueue.front();
                stickerQueue.pop();
                stickDesk.isAvailable = false;
                hacker2->deskId = stickDesk.id;
                hacker2->waitingTimeSticker += (time - hacker2->entranceTimeStQ);
                waitingTimeStQ += (time - hacker2->entranceTimeStQ);
                heapOfEvents.push(Event("LeaveStDesk", time + stickDesk.serviceTime, hacker2->id));
            }
        }
        else if(event.type == "LeaveHoodDesk"){//hacker leaves hoodie desk
            Hacker &hacker = hackersList[event.hackerID -1];
            Desk &hoodDesk = hoodieDesks[hacker.deskId];
            hoodDesk.isAvailable = true;
            hacker.numOfGifts++;
            //hacker.turnAroundTimeCount += (time - hacker.entranceTimeStQ);
            turnAroundTimes += (time - hacker.entranceTimeStQ);

            //if the hoodie queue is not empty, one of the hackers goes into the newly available hoodie desk
            if(!hoodieQueue.empty()){
                Hacker* hacker2 = hoodieQueue.top();
                hoodieQueue.pop();
                hoodDesk.isAvailable = false;
                hacker2->deskId = hoodDesk.id;
                waitingTimeHoQ += (time - hacker2->entranceTimeHoodQ);
                hacker2->waitingTimeHoodie += (time - hacker2->entranceTimeHoodQ);
                heapOfEvents.push(Event("LeaveHoodDesk", time + hoodDesk.serviceTime, hacker2->id));
            }
        }



    }

        outfile<< maxSizeStickerQ << endl;//1 maximum length of sticker queue
        outfile<< maxSizeHoodieQ << endl;//2 Maximum length of the hoodie queue.
        float numGifts = 0;
        float totalChangeCommits = 0;
        int idWaitingTheMost = -1;
        float maxWaiting = -1;
        int idLeastWait = 1;
        float minWaitingTime = numeric_limits<float>::max();

        for (int i = 0; i < hackersList.size(); i++) {
            numGifts += hackersList[i].numOfGifts;
            totalChangeCommits += hackersList[i].totalLengthCommits;

            //checking for the one who waited the most
            float waitingTimeTotal = hackersList[i].waitingTimeHoodie + hackersList[i].waitingTimeSticker;
            if(waitingTimeTotal - maxWaiting > 0.00001){
                maxWaiting = waitingTimeTotal;
                idWaitingTheMost = hackersList[i].id;
            }
            //finding the one who waited least and got three gifts
            if(hackersList[i].numOfGifts == 3){
                if(minWaitingTime - waitingTimeTotal > 0.00001){
                    minWaitingTime = waitingTimeTotal;
                    idLeastWait = hackersList[i].id;
                }
            }

        }
        if(hackersList[idLeastWait - 1].numOfGifts != 3){
            idLeastWait = -1;
            minWaitingTime = -1;
        }
        outfile<< fixed << setprecision(3) << float(numGifts / hackersList.size()) << endl;//3 Average number of gifts grabbed per hacker.
        outfile << fixed << setprecision(3) << float(waitingTimeStQ / numGifts) << endl;//4 Average waiting time in the sticker queue.
        outfile << fixed << setprecision(3) << float(waitingTimeHoQ / numGifts) << endl;//5 Average waiting time in the hoodie queue

        outfile<< fixed << setprecision(3) << float(numCodeCommits / hackersList.size()) << endl;//6 Average number of code commits per hacker.

        outfile<< fixed << setprecision(3) << float(totalChangeCommits / numCodeCommits) << endl;//7 Average change length of the commits
        outfile << fixed << setprecision(3) << float(turnAroundTimes / numGifts) << endl;//8 Average turnaround time
        outfile<< countInvAtStQ << endl;//9 Total number of invalid attempts to enter sticker queue.
        outfile<< countInvAttEnoughGifts << endl;//10  Total number of invalid attempts to get more than 3 gifts.
        outfile << idWaitingTheMost << " " << fixed << setprecision(3) << float(maxWaiting) << endl; // 11 ID of the hacker who spent the most time in the queues and the waiting time
        outfile << idLeastWait << " " << fixed << setprecision(3) << float(minWaitingTime) << endl;// 12 ID of the hacker who spent the least time in the queues
        outfile<< fixed << setprecision(3) << float(time);// Total seconds passed during the hackathon.

    return 0;
}
