#include "fileParser.h"
#include "ctype.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string removeAllPoli(std::string polymer){
    for(size_t i = 0; (i+1) < polymer.size(); ++i){
        char tempChangeCase = ' ';
        if(isupper(polymer[i])){
            tempChangeCase = tolower(polymer[i]);
        } else {
            tempChangeCase = toupper(polymer[i]); 
        }
        if(tempChangeCase == polymer[i+1]){
            polymer.erase(i,2);
            i -= 2;
        }
    }
    return polymer;
}

char getWorstUnit(std::string polymer){
    std::map<char,int> letterUsage;
    for(size_t i = 0; i  < polymer.size(); ++i){
        letterUsage[tolower(polymer[i])] += 1;
    }
    auto pr = std::max_element(letterUsage.begin(), letterUsage.end(), [](const std::pair<char,int>& p1, const std::pair<char,int>& p2){return p1.second < p2.second;});
    return pr->first;
}

std::string removeAllWorst(std::string polymer, char worst){
    polymer.erase(std::remove(polymer.begin(), polymer.end(), tolower(worst)), polymer.end());
    polymer.erase(std::remove(polymer.begin(), polymer.end(), toupper(worst)), polymer.end());
    return polymer;
}


int getLowestSize(std::string polymer){
    int lowestSize = 9000000;
    std::string alpha = "abcdefghijklmnopqrstuvwxyz";
    for(size_t i = 0; i < alpha.size(); ++i){
        int curSize = removeAllPoli(removeAllWorst(polymer,alpha[i])).size(); 
        if (curSize < lowestSize){
            lowestSize = curSize;
        }
    }
    return lowestSize;
    
}

int main(){
    std::vector<std::string> polymerContainer = fileParse::storeEachLine("./challenges/challenge5/input.txt");
    std::string polymer = polymerContainer.front();
    std::string newPolymer = removeAllPoli(polymer);
    std::cout << "new size: " << newPolymer.size() << "\n";;
    std::cout << "most used: " << getLowestSize(polymer) << "\n";
    return 0;
}
