#include "fileParser.h"
#include <iostream>
#include <regex>

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
    }
    return stepsWithPre;
}

int main(){
    //std::map<char, bool> stepsDone;
    std::vector<std::string> allSteps = fileParse::storeEachLine("./challenges/challenge7/input.txt");
    std::map<char,std::vector<char> > stepsPreReq = getStepsWithPre(allSteps);
    for(const auto& step : stepsPreReq){
        std::cout << step.first << ": ";
        for(const auto& pre : step.second){
            std::cout << pre << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
