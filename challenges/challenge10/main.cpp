#include "fileParser.h"
#include <iostream>
#include <regex>

typedef std::pair<int,int> Point;

struct star{
    star(int xPos, int yPos, int xVel, int yVel):xPos(xPos),yPos(yPos),xVel(xVel),yVel(yVel){}
    bool operator ==(const star& rhs){
        return ((xPos == rhs.xPos) && (yPos == rhs.yPos));
    }
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

int getMinX(const std::vector<star>& sky){
    return std::min_element(sky.begin(),sky.end(),[](const auto& p1, const auto& p2){ return p1.xPos < p2.xPos;})->xPos;
}

int getMinY(const std::vector<star>& sky){
    return std::min_element(sky.begin(), sky.end(), [](const auto& p1, const auto& p2){ return p1.yPos < p2.yPos;})->yPos;
}

int getMaxX(const std::vector<star>& sky){
    return std::max_element(sky.begin(), sky.end(), [](const auto& p1, const auto& p2){ return p1.xPos < p2.xPos;})->xPos;
}

int getMaxY(const std::vector<star>& sky){
    return std::max_element(sky.begin(), sky.end(), [](const auto& p1, const auto& p2){ return p1.yPos < p2.yPos;})->yPos;
}

void printCurrentSky(std::vector<star>& sky){
    int minX = getMinX(sky);
    int minY = getMinY(sky);
    int maxX = getMaxX(sky);
    int maxY = getMaxY(sky);
    for(int y = minY; y <= maxY; ++y) {
        for(int x = minX; x <= maxX; ++x) {
            if(std::find(sky.begin(), sky.end(), star(x,y,0,0))!=sky.end()){
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n\n\n\n\n";
}

void incrementStars(std::vector<star>& sky){
    for(auto& star : sky){
        star.xPos += star.xVel;
        star.yPos += star.yVel;
    }
}

bool adjacent(std::vector<star>& sky){
    for(const auto& currentStar : sky){
        if(std::find(sky.begin(),sky.end(),star((currentStar.xPos-1),currentStar.yPos,0,0))!=sky.end()){
            continue;
        }
        if(std::find(sky.begin(),sky.end(),star((currentStar.xPos+1),currentStar.yPos,0,0))!=sky.end()){
            continue;
        }
        if(std::find(sky.begin(),sky.end(),star(currentStar.xPos,(currentStar.yPos-1),0,0))!=sky.end()){
            continue;
        }
        if(std::find(sky.begin(),sky.end(),star(currentStar.xPos,(currentStar.yPos+1),0,0))!=sky.end()){
            continue;
        }
        if(std::find(sky.begin(),sky.end(),star((currentStar.xPos-1),(currentStar.yPos+1),0,0))!=sky.end()){
            continue;
        }
        if(std::find(sky.begin(),sky.end(),star((currentStar.xPos+1),(currentStar.yPos+1),0,0))!=sky.end()){
            continue;
        }
        if(std::find(sky.begin(),sky.end(),star((currentStar.xPos-1),(currentStar.yPos-1),0,0))!=sky.end()){
            continue;
        }
        if(std::find(sky.begin(),sky.end(),star((currentStar.xPos+1),(currentStar.yPos-1),0,0))!=sky.end()){
            continue;
        }

        return false;
    }
    return true;
}


int main(){
    std::vector<std::string> starsBegin = fileParse::storeEachLine("./challenges/challenge10/input.txt");
    std::vector<star> sky = parseStars(starsBegin);
    long count = 0;
    long pacer = 1000000;
    while(true){
        if(count > pacer){
            std::cout << count << "\n";
            pacer += 1000000;
        }
        if(!adjacent(sky)){
            incrementStars(sky);
        } else{
            std::cout << count << "\n";
            printCurrentSky(sky);
            break;
        }
        ++count;
    }
    return 0;
}
