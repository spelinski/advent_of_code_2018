#include "grid.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

typedef std::pair<int,int> Point;

int SerialNumber = 3628;
grid::grid<int> PowerLevelGrid;

//These two are used for memoization
std::map<Point, std::map<int,int> > PowerLevelSizeMap;
std::map<Point, std::map<int,bool> > PowerLevelSizeCheck;

void seedMap(){
    for(int y = 1; y <= 300; ++y){
        for(int x = 1; x <= 300; ++x){
            int rackId = x + 10;
            int localPowerLevel = rackId * y;
            localPowerLevel += SerialNumber;
            localPowerLevel *= rackId;
            std::string powerString = std::to_string(localPowerLevel);
            std::reverse(powerString.begin(), powerString.end());
            if(powerString.size() >= 3){
                std::string hundreds(1,powerString.at(2));
                localPowerLevel = std::stoi(hundreds);
            } else {
                localPowerLevel = 0;
            }
            localPowerLevel -= 5;
            PowerLevelGrid.setItem(Point(x,y), localPowerLevel);
        }
    }
}

int getPowerLevel(Point upperLeft, int size=3){
    int powerLevel = 0;
    if(PowerLevelSizeCheck[upperLeft][size-1]){
        powerLevel = PowerLevelSizeMap[upperLeft][size-1];
        for(int x = upperLeft.first; x < (upperLeft.first+size); ++x){
            powerLevel += PowerLevelGrid.getItem(Point(x,(upperLeft.second+(size-1))));
        }
        for(int y = upperLeft.second; y < (upperLeft.second+size-1); ++y){
            powerLevel += PowerLevelGrid.getItem(Point((upperLeft.first+(size-1)),y));
        }
    } else {
        for(int y = upperLeft.second; y < (upperLeft.second+size); ++y){
            for(int x = upperLeft.first; x < (upperLeft.first+size); ++x){
                powerLevel += PowerLevelGrid.getItem(Point(x,y));
            }
        }
    }
    PowerLevelSizeMap[upperLeft][size]=powerLevel;
    PowerLevelSizeCheck[upperLeft][size]=true;
    return powerLevel;
}

int main(){
    seedMap();
    int maxPowerLevel = 0;
    Point maxPoint = Point(0,0);
    for(int y = 1; y <= 298; ++y){
        for(int x = 1; x <= 298; ++x){
            int currentLevel = getPowerLevel(Point(x,y));
            if(currentLevel > maxPowerLevel){
                maxPowerLevel = currentLevel;
                maxPoint = Point(x,y);
            }
        }
    }
    std::cout << "Point: " << maxPoint.first << "," << maxPoint.second << "\n";

    
    maxPowerLevel = 0;
    maxPoint = Point(0,0);
    int maxSize = 0;
    for(int currentSize = 1; currentSize <= 300; ++currentSize){
        for(int y = 1; y <= (300-(currentSize-1)); ++y){
            for(int x = 1; x <= (300-(currentSize-1)); ++x){
                int currentLevel = getPowerLevel(Point(x,y),currentSize);
                if(currentLevel > maxPowerLevel){
                    maxPowerLevel = currentLevel;
                    maxPoint = Point(x,y);
                    maxSize = currentSize;
                }
            }
        }
        std::cout << "currentSize: " << currentSize << "\n";
    }
    std::cout << "Point: " << maxPoint.first << "," << maxPoint.second << "," << maxSize << "\n";
    std::cout << "power: " << maxPowerLevel << "\n";

    return 0;
}
