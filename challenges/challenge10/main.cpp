#include "fileParser.h"
#include <iostream>
#include <regex>

struct star{
    star(int xPos, int yPos, int xVel, int yVel):xPos(xPos),yPos(yPos),xVel(xVel),yVel(yVel){}
    int xPos;
    int yPos;
    int xVel;
    int yVel;
};

std::vector<star> parseStars(std::vector<std::string> allStarStrings){
    std::vector<star> output;
    std::regex starReg (".*<\\s*(.*),\\s*(.*)>.*<\\s*(.*),\\s*(.*)");
    for(const auto& currentStar : allStarStrings){
        std::smatch myMatch;
        std::regex_search(currentStar, myMatch, starReg);
        output.push_back(star(std::stoi(myMatch[1]),std::stoi(myMatch[2]),std::stoi(myMatch[3]),std::stoi(myMatch[4])));
    }
    return output;
}


int main(){
    std::vector<std::string> starsBegin = fileParse::storeEachLine("./challenges/challenge10/input.txt");
    std::vector<star> sky = parseStars(starsBegin);
    for(const auto& currStar : sky){
        std::cout << currStar.xPos << "," << currStar.yPos << " : " << currStar.xVel << "," << currStar.yVel << "\n";
    }
    return 0;
}
