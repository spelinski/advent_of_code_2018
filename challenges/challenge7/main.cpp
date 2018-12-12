#include "fileParser.h"
#include <iostream>
#include <regex>

static std::map<char,int> abcToNum = {{'A',1},{'B',2},{'C',3},{'D',4},{'E',5},{'F',6},{'G',7},{'H',8},{'I',9},{'J',10},{'K',11},{'L',12},{'M',13},{'N',14},{'O',15},{'P',16},{'Q',17},{'R',18},{'S',19},{'T',20},{'U',21},{'V',22},{'W',23},{'X',24},{'Y',25},{'Z',26}};

std::map<char, std::vector<char> > getStepsWithPre(std::vector<std::string> allSteps) {
    std::regex stepReg ("[A-Z|a-z]+\\s+([A-Z]).*([A-Z])");
    std::map<char, std::vector<char> > stepsWithPre;
    for(const auto& inst : allSteps){ 
        std::smatch myMatch;
        std::regex_search(inst, myMatch, stepReg);
        std::string currStepString = myMatch[2];
        char currStep = currStepString[0];
        std::string preqStepString = myMatch[1];
        char preqStep = preqStepString[0];
        stepsWithPre[currStep].push_back(preqStep);
        stepsWithPre[preqStep];
    }
    return stepsWithPre;
}

bool areAllDone(std::map<char, std::vector<char> > allSteps, std::map<char, bool> stepsDone){
    for(const auto& step : allSteps){
        if(!stepsDone[step.first]){return false;}
    }
    return true;
}

bool arePreDone(std::vector<char> preReqs, std::map<char, bool> stepsDone) {
    for(const auto& step : preReqs){
        if(!stepsDone[step]){return false;}
    }
    return true;
}

std::string getOrder(std::map<char, std::vector<char> > stepsWithPreReq) {
    std::string output = "";
    std::map<char, bool> stepsDone;
    while(!areAllDone(stepsWithPreReq, stepsDone)){
        for(const auto& step : stepsWithPreReq) {
            if(!stepsDone[step.first] && arePreDone(step.second, stepsDone)){
                output += step.first;
                stepsDone[step.first] = true;
                break;
            }
        }
    }
    return output;
}

struct worker{
    public:
        explicit worker():working(false),timeRemaining(0),stepOn(' '){}
        bool operator < (const worker& rhs){
            if(!working){return false;}
            if(!rhs.working) {return true;}
            return timeRemaining < rhs.timeRemaining;
        }
        bool working;
        int timeRemaining;
        char stepOn;
};

bool otherWorkerAlreadyOn(char step, std::vector<worker> workers){
    for(const auto& singleWorker : workers){
        if(singleWorker.stepOn == step){
            return true;
        }
    }
    return false;
}

int getTime(std::map<char, std::vector<char> > stepsWithPreReq) {
    std::vector<worker> workers;
    workers.push_back(worker());
    workers.push_back(worker());
    workers.push_back(worker());
    workers.push_back(worker());
    workers.push_back(worker());
    std::map<char, bool> stepsDone;
    int timeGone = 0;
    while(!areAllDone(stepsWithPreReq, stepsDone)){
        for(const auto& step : stepsWithPreReq) {
            if(!stepsDone[step.first] && arePreDone(step.second, stepsDone)){
                for(auto& singleWorker : workers){
                    if(!singleWorker.working && !otherWorkerAlreadyOn(step.first, workers)){
                        singleWorker.stepOn = step.first;
                        singleWorker.working = true;
                        singleWorker.timeRemaining = abcToNum[step.first] + 60;
                        break;
                    }
                }
            }
        }
        auto pr = std::min_element(workers.begin(),workers.end());
        int minTimeRemaining = pr->timeRemaining;
        for(auto& singleWorker : workers){
            if(singleWorker.timeRemaining == minTimeRemaining){ 
                stepsDone[singleWorker.stepOn] = true;
                singleWorker.working = false;
                singleWorker.timeRemaining = 0;
            } else if(singleWorker.working) {
                singleWorker.timeRemaining -= minTimeRemaining;
            }
        }
        timeGone += minTimeRemaining;
    }
    return timeGone;
}

int main(){
    std::vector<std::string> allSteps = fileParse::storeEachLine("./challenges/challenge7/input.txt");
    std::map<char,std::vector<char> > stepsPreReq = getStepsWithPre(allSteps);
    std::cout << "order: " << getOrder(stepsPreReq) << "\n";
    std::cout << "time: " << getTime(stepsPreReq) << "\n";
    return 0;
}
