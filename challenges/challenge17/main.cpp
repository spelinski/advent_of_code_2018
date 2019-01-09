#include "fileParser.h"
#include "grid.h"
#include <iostream>
#include <regex>
#include <variant>

struct sand{
    sand(){}
};
struct clay{};
struct water{
    water():settled(false),fallingForever(false),movingLeft(false),movingRight(false){}
    bool settled;
    bool fallingForever;
    bool movingLeft;
    bool movingRight;
};

template<class... Ts> struct overloaded : Ts... {using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
using underObjects = std::variant<sand, clay, water>;

class underground {
    public:
        underground():minX(501),maxX(0),minY(1000),finished(false){}
        void printUnderGround(std::ostream& outputStream){
            int maxY = getMaxY();
            for(int y = 0; y <= maxY; ++y){
                for(int x = minX; x <= maxX; ++x){
                    underObjects underObj = undergroundMap.getItem({x,y});
                    std::visit(overloaded {
                        [&outputStream](const sand&){outputStream << ".";},
                        [&outputStream](const clay&){outputStream << "#";},
                        [&outputStream](const water& aqua){
                            if(aqua.fallingForever){outputStream <<"|";}
                            else{outputStream << "~";}
                        }
                    }, underObj);
                }
                outputStream << "\n";
            }
        }
        void dropWater(){
            if(!std::holds_alternative<sand>(undergroundMap.getItem({500,1}))){
                finished = true;
                return;
            }

            int maxY = getMaxY();
            water newDrop;
            grid::Point currentLocation = {500,1};
            while(!newDrop.settled && !newDrop.fallingForever && (currentLocation.second <= maxY)){
                if(currentLocation.second == maxY){
                    newDrop.fallingForever = true;
                } else if(checkDown(currentLocation)){
                    newDrop.movingLeft = false;
                    newDrop.movingRight = false;
                    currentLocation = {currentLocation.first, currentLocation.second+1};
                } else if(newDrop.movingLeft && checkLeft(currentLocation)){
                    currentLocation = {currentLocation.first-1, currentLocation.second};
                } else if(newDrop.movingRight && checkRight(currentLocation)){
                    currentLocation = {currentLocation.first+1, currentLocation.second};
                } else if((!newDrop.movingLeft && !newDrop.movingRight) && (checkLeft(currentLocation) || checkRight(currentLocation))){
                    bool isLeftValid = checkLeft(currentLocation);
                    bool isRightValid = checkRight(currentLocation);
                    if(isLeftValid && !isRightValid){
                        newDrop.movingLeft = true;
                        currentLocation = {currentLocation.first-1, currentLocation.second};
                    } else if(!isLeftValid && isRightValid){
                        newDrop.movingRight = true;
                        currentLocation = {currentLocation.first+1, currentLocation.second};
                    } else if(!rightIsNotSandBeforeDown(currentLocation)){
                        newDrop.movingRight = true;
                        currentLocation = {currentLocation.first+1, currentLocation.second};
                    } else {
                        newDrop.movingLeft = true;
                        currentLocation = {currentLocation.first-1, currentLocation.second};
                    }
                }else {
                    if(checkForSettle(currentLocation)) {
                        newDrop.settled = true;
                    } else {
                        newDrop.fallingForever = true;
                    }
                }
            }
            if(currentLocation.first < minX){minX = currentLocation.first;}
            else if(currentLocation.first > maxX){maxX = currentLocation.first;}
            undergroundMap.setItem(currentLocation, newDrop);
        }
        int getWaterCount(){
            int count = 0;
            for(const auto& underObj : undergroundMap){
                if(std::holds_alternative<water>(underObj.second)){
                    int y = underObj.first.second;
                    if(y >= minY && y <= getMaxY()){
                        ++count;
                    }
                }
            }
            return count;
        }
        int getSettledWaterCount(){
            int count = 0;
            for(const auto& underObj : undergroundMap){
                if(std::holds_alternative<water>(underObj.second)){
                    int y = underObj.first.second;
                    if(y >= minY && y <= getMaxY()){
                        if(std::get<water>(underObj.second).settled){
                            ++count;
                        }
                    }
                }
            }
            return count;
        }
        int getMaxY(){return undergroundMap.rbegin()->first.second;}
        int minX;
        int maxX;
        int minY;
        bool finished;
        grid::grid<underObjects> undergroundMap;
    private:
        bool checkDown(grid::Point currentPoint){
            int x = currentPoint.first;
            int y = currentPoint.second;
            return std::holds_alternative<sand>(undergroundMap.getItem({x,y+1}));
        }
        bool checkLeft(grid::Point currentPoint){
            int x = currentPoint.first;
            int y = currentPoint.second;
            bool fallingForever = isRunningWater(x,y+1);
            return std::holds_alternative<sand>(undergroundMap.getItem({x-1,y})) && !fallingForever;
        }
        bool checkRight(grid::Point currentPoint){
            int x = currentPoint.first;
            int y = currentPoint.second;
            bool fallingForever = isRunningWater(x,y+1);
            return std::holds_alternative<sand>(undergroundMap.getItem({x+1,y})) && !fallingForever;
        }
        bool isSettledWater(int x, int y){
            if(std::holds_alternative<water>(undergroundMap.getItem({x,y}))){
                if(!std::get<water>(undergroundMap.getItem({x,y})).fallingForever){
                    return true;
                }
            }
            return false;
        }
        bool isRunningWater(int x, int y){
            if(std::holds_alternative<water>(undergroundMap.getItem({x,y}))){
                if(std::get<water>(undergroundMap.getItem({x,y})).fallingForever){
                    return true;
                }
            }
            return false;
        }

        bool checkForSettle(grid::Point currentPoint){
            int x = currentPoint.first;
            int y = currentPoint.second;
            bool belowIsClay = std::holds_alternative<clay>(undergroundMap.getItem({x,y+1}));
            bool belowIsSettledWater = isSettledWater(x,y+1);
            if(!belowIsClay && !belowIsSettledWater){
                return false;
            }
            bool leftIsRunningWater = isRunningWater(x-1,y);
            bool rightIsRunningWater = isRunningWater(x+1,y);

            if(leftIsRunningWater || rightIsRunningWater){
                return false;
            }

            if(!leftIsNotSandBeforeDown(currentPoint)){
                return false;
            }
            if(!rightIsNotSandBeforeDown(currentPoint)){
                return false;
            }
            return true;
        }

        bool leftIsNotSandBeforeDown(grid::Point currentPoint){
            int x = currentPoint.first;
            int y = currentPoint.second;
            while(true){
                --x;
                if(isSettledWater(x,y)){
                    return true;
                } else if(std::holds_alternative<clay>(undergroundMap.getItem({x,y}))){
                    return true;
                } else if(checkDown({x,y})){
                    return false;
                }
            }
        }
        bool rightIsNotSandBeforeDown(grid::Point currentPoint){
            int x = currentPoint.first;
            int y = currentPoint.second;
            while(true){
                ++x;
                if(isSettledWater(x,y)){
                    return true;
                } else if(std::holds_alternative<clay>(undergroundMap.getItem({x,y}))){
                    return true;
                } else if(checkDown({x,y})){
                    return false;
                }
            }
        }


};

underground parseOutClay(std::vector<std::string> clayLines){
    underground outputMap;
    for(const auto& line : clayLines){
        std::regex clayReg("([x|y])=([0-9]+),\\s+[x|y]=([0-9]+)..([0-9]+)"); 
        std::smatch myMatch;
        std::regex_search(line, myMatch, clayReg);
        std::string xOrYFirst = myMatch[1];
        int xfirst = 0;
        int xsecond = 0;
        int yfirst = 0;
        int ysecond = 0;
        if(xOrYFirst == "x"){
            xfirst = xsecond = std::stoi(myMatch[2]);
            yfirst = std::stoi(myMatch[3]);
            ysecond = std::stoi(myMatch[4]);
        } else {
            yfirst = ysecond = std::stoi(myMatch[2]);
            xfirst = std::stoi(myMatch[3]);
            xsecond = std::stoi(myMatch[4]);
        }
        for(int x = xfirst ; x <= xsecond; ++x){
            if(x < outputMap.minX){outputMap.minX = x;}
            else if ( x > outputMap.maxX){outputMap.maxX = x;}
            for(int y = yfirst; y <= ysecond; ++y){
                if(y <outputMap.minY){outputMap.minY = y;}
                outputMap.undergroundMap.setItem({x,y},clay());
            }
        } 
    }
    return outputMap;
}

int main(){
    std::vector<std::string> clayLines = fileParse::storeEachLine("./challenges/challenge17/input.txt");
    underground waterScan = parseOutClay(clayLines);
    waterScan.printUnderGround(std::cout);
    while(!waterScan.finished){
        waterScan.dropWater();
    }
    std::cout << "\n\n";
    waterScan.printUnderGround(std::cout);
    std::cout << "\n\n";
    std::cout << "count of water: " << waterScan.getWaterCount() << "\n";
    std::cout << "count of settled water: " << waterScan.getSettledWaterCount() << "\n";
    return 0;
}
