#include "elfPlan.h"
#include "fileParser.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

elfPlan::elfPlanHandler getElfPlans(const std::vector<std::string>& plans) {
    elfPlan::elfPlanHandler output;
    
    std::regex planReg ("#([0-9]+)\\s+@\\s+([0-9]+),([0-9]+)+:\\s+([0-9]+)x([0-9]+)");
    for( const auto& plan : plans ) {
        std::smatch myMatch;
        std::regex_search(plan,myMatch,planReg);
        int id = std::stoi(myMatch.str(1));
        elfPlan::Point startPoint = elfPlan::Point(std::stoi(myMatch.str(2)),std::stoi(myMatch.str(3)));
        int xDistance = std::stoi(myMatch.str(4));
        int yDistance = std::stoi(myMatch.str(5));
        output.insert(elfPlan::elfPlan(elfPlan::planId(id), elfPlan::initPoint(startPoint), elfPlan::xDist(xDistance), elfPlan::yDist(yDistance)));
    }

    return output;
}

int main() {
    std::vector<std::string> allPlans = fileParse::storeEachLine("./challenges/challenge3/input.txt");
    elfPlan::elfPlanHandler elfPlans = getElfPlans(allPlans);
    int inchDups = elfPlans.getInchesDuplicated();
    std::cout << "Dup inches: " << inchDups << "\n";
    int idOfUnique = elfPlans.getUniquePlan();
    std::cout << "unique id: " << idOfUnique << "\n";
    return 0;
}
