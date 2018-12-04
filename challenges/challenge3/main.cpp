#include "elfPlan.h"
#include "fileParser.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>

std::vector<elfPlan::elfPlan> getElfPlans(const std::vector<std::string>& plans) {
    std::vector<elfPlan::elfPlan> outputVector;
    
    std::regex planReg ("#([0-9]+)\\s+@\\s+([0-9]+),([0-9]+)+:\\s+([0-9]+)x([0-9]+)");
    for( const auto& plan : plans ) {
        std::smatch myMatch;
        std::regex_search(plan,myMatch,planReg);
        int id = std::stoi(myMatch.str(1));
        std::pair<int,int> startPoint = std::pair<int,int>(std::stoi(myMatch.str(2)),std::stoi(myMatch.str(3)));
        int xDistance = std::stoi(myMatch.str(4));
        int yDistance = std::stoi(myMatch.str(5));
        outputVector.push_back(elfPlan::elfPlan(elfPlan::planId(id), elfPlan::initPoint(startPoint), elfPlan::xDist(xDistance), elfPlan::yDist(yDistance)));
    }

    return outputVector;
}

std::map<std::pair<int,int>, int> getClothUsage(std::vector<elfPlan::elfPlan> planVector) {
    std::map<std::pair<int,int>, int> outputMap;

    for( const auto& plan : planVector ) {
        int startX = plan.getStartPoint().first;
        int startY = plan.getStartPoint().second;
        int distanceX = plan.getXDistance();
        int distanceY = plan.getYDistance();;
        for(int x = startX; x < (startX+distanceX); ++x) {
            for(int y = startY; y < (startY+distanceY); ++y) {
                outputMap[std::pair<int,int>(x,y)] += 1;
            }
        }
    }
    return outputMap;
}

bool isDuplicated(std::pair<std::pair<int,int>, int> inch) {
    return (inch.second > 1);
}

int getUniquePlan(std::map<std::pair<int,int>, int> usageMap, std::vector<elfPlan::elfPlan> allElfPlans) {
    for(const auto& plan : allElfPlans) {
        bool unique = true;
        int startX = plan.getStartPoint().first;
        int startY = plan.getStartPoint().second;
        int distanceX = plan.getXDistance();
        int distanceY = plan.getYDistance();
        for(int x = startX; x < (startX+distanceX); ++x) {
            for(int y = startY; y < (startY+distanceY); ++y) {
                if(usageMap[std::pair<int,int>(x,y)] > 1) {
                    unique = false;
                }
            }
        }
        if(unique) {
            return plan.getId();
        }
    }
    return 0;
}

int main() {
    std::vector<std::string> allPlans = fileParse::storeEachLine("./challenges/challenge3/input.txt");
    std::vector<elfPlan::elfPlan> planMap = getElfPlans(allPlans);
    std::map<std::pair<int,int>, int> usage = getClothUsage(planMap);
    int inchDups = std::count_if(usage.begin(),usage.end(),isDuplicated);
    std::cout << "Dup inches: " << inchDups << "\n";
    int idOfUnique = getUniquePlan(usage, planMap);
    std::cout << "unique id: " << idOfUnique << "\n";
    return 0;
}
