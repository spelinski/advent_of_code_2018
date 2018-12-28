#include "grid.h"
#include "fileParser.h"
#include <iostream>
#include <list>
#include <variant>

enum fighterType {
    GOBLIN,
    ELF
};

enum direction {
    UP = 'U' ,
    DOWN = 'D',
    LEFT = 'L',
    RIGHT = 'R'
};

struct goblinFighter{
    public:
        goblinFighter():alreadyMoved(false),health(200),attackPower(3){}
        bool alreadyMoved;
        int health;
        int attackPower;
};

struct elfFighter{
        elfFighter():alreadyMoved(false),health(200),attackPower(3){}
        bool alreadyMoved;
        int health;
        int attackPower;
};

struct wall{};

struct empty{};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using caveSpot = std::variant<goblinFighter, elfFighter, wall, empty>;

bool isValid(grid::Point testPoint, grid::grid<caveSpot> encounterMap){
    return (testPoint.first >= 0) && (testPoint.second >= 0) && (testPoint.first <= encounterMap.rbegin()->first.first) && (testPoint.second <= encounterMap.rbegin()->first.second);
}

struct pathNode{
    grid::Point location;
    int distance;
    direction firstStep;
};

pathNode findShortestPath(grid::Point startPoint, grid::Point endPoint, grid::grid<caveSpot>& encounterMap){
    grid::grid<bool> visited;
    for(const auto& spot : encounterMap){
        visited.setItem(spot.first, false);
    }

    std::list<pathNode> currentQueue;
    pathNode currentSpot;
    currentSpot.location = startPoint;
    currentSpot.distance = 0;
    currentQueue.push_back(currentSpot);
    visited.setItem(startPoint, true);

    while(!currentQueue.empty()){
        auto currentPoint = currentQueue.front();
        currentQueue.pop_front();
        if(currentPoint.location == endPoint) {
            return currentPoint;
        }

        grid::Point nextPoint(currentPoint.location.first,currentPoint.location.second-1);
        //up
        if(isValid(nextPoint,encounterMap) && (!visited.getItem(nextPoint))){
                pathNode tempNode;
                tempNode.distance = currentPoint.distance + 1;
                if(currentPoint.distance > 0){
                    tempNode.firstStep = currentPoint.firstStep;
                } else {
                    tempNode.firstStep = direction::UP;
                }
                tempNode.location = nextPoint;
                visited.setItem(tempNode.location, true);
                if((tempNode.location == endPoint) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))){
                    currentQueue.push_back(tempNode);
                }
        }

        nextPoint = {currentPoint.location.first-1,currentPoint.location.second};
        //left
        if(isValid(nextPoint,encounterMap) && (!visited.getItem(nextPoint))){
            pathNode tempNode;
            tempNode.distance = currentPoint.distance + 1;
            if(currentPoint.distance > 0){
                tempNode.firstStep = currentPoint.firstStep;
            } else {
                tempNode.firstStep = direction::LEFT;
            }
            tempNode.location = nextPoint;
            visited.setItem(tempNode.location, true);
            if(tempNode.location == endPoint || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))){
                currentQueue.push_back(tempNode);
            }
        }

        nextPoint = {currentPoint.location.first+1,currentPoint.location.second};
        //right
        if(isValid(nextPoint,encounterMap) && (!visited.getItem(nextPoint))){
                pathNode tempNode;
                tempNode.distance = currentPoint.distance + 1;
                if(currentPoint.distance > 0){
                    tempNode.firstStep = currentPoint.firstStep;
                } else {
                    tempNode.firstStep = direction::RIGHT;
                }
                tempNode.location = nextPoint;
                visited.setItem(tempNode.location, true);
                if(tempNode.location == endPoint || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))){
                    currentQueue.push_back(tempNode);
                }
        }

        nextPoint = {currentPoint.location.first, currentPoint.location.second+1};
        //down
        if(isValid(nextPoint,encounterMap) && (!visited.getItem(nextPoint))){
                pathNode tempNode;
                tempNode.distance = currentPoint.distance + 1;
                if(currentPoint.distance > 0){
                    tempNode.firstStep = currentPoint.firstStep;
                } else {
                    tempNode.firstStep = direction::DOWN;
                }
                tempNode.location = nextPoint;
                visited.setItem(tempNode.location, true);
                if(tempNode.location == endPoint || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))){
                    currentQueue.push_back(tempNode);
                }
        } 
    }
    return currentSpot;
}

pathNode findPathToNearestElf(const grid::Point& startPoint, grid::grid<caveSpot>& encounterMap){
    pathNode minimumPath;
    minimumPath.distance = 0;
    for(const auto& spot : encounterMap){
        if(std::holds_alternative<elfFighter>(spot.second)){
            pathNode tempNode = findShortestPath(startPoint, spot.first, encounterMap);
            if((tempNode.distance < minimumPath.distance || minimumPath.distance==0) && tempNode.distance > 0){
                minimumPath = tempNode;
            }
        }
    }
    return minimumPath;
}

pathNode findPathToNearestGoblin(const grid::Point& startPoint, grid::grid<caveSpot>& encounterMap){
    pathNode minimumPath;
    minimumPath.distance = 0;
    for(const auto& spot : encounterMap){
        if(std::holds_alternative<goblinFighter>(spot.second)){
            pathNode tempNode = findShortestPath(startPoint, spot.first, encounterMap);
            if((tempNode.distance < minimumPath.distance || minimumPath.distance==0) && tempNode.distance > 0){
                minimumPath = tempNode;
            }
        }
    }
    return minimumPath;
}

struct cave{
    cave(){}
    void outputCave(){
        int currentY = 0;
        for(const auto& feature : caveFeatures){
            if(feature.first.second > currentY){
                currentY = feature.first.second;
                std::cout << "\n";
            }
            std::visit(overloaded {
                [](const goblinFighter&) {std::cout << "G";},
                [](const elfFighter&) {std::cout << "E";},
                [](const wall&) {std::cout << "#";},
                [](const empty&) {std::cout << ".";}
            }, feature.second);
        }
        std::cout << "\n";
    }

    void peformTurn(){
        grid::grid<caveSpot> tempCaveFeatures = caveFeatures;
        for(const auto& feature : caveFeatures){
            caveSpot tempCaveSpot = tempCaveFeatures.getItem(feature.first);
            std::visit(overloaded {
                //This isn't meant to work yet
                [&tempCaveFeatures, &feature](goblinFighter gf){
                    if(!gf.alreadyMoved){
                        gf.alreadyMoved = true;
                        pathNode pathToElf = findPathToNearestElf(feature.first, tempCaveFeatures);
                        tempCaveFeatures.setItem(grid::Point(feature.first.first, feature.first.second),empty());
                        if(pathToElf.firstStep == direction::UP && pathToElf.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first,feature.first.second-1),gf);
                        } else if(pathToElf.firstStep == direction::LEFT && pathToElf.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first-1,feature.first.second),gf);
                        } else if(pathToElf.firstStep == direction::RIGHT && pathToElf.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first+1,feature.first.second),gf);
                        } else if(pathToElf.firstStep == direction::DOWN && pathToElf.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first,feature.first.second+1),gf);
                        } else {
                            tempCaveFeatures.setItem(feature.first,gf);
                        }
                        if(pathToElf.distance == 1 || pathToElf.distance == 2){
                            elfFighter ef = std::get<elfFighter>(tempCaveFeatures.getItem(pathToElf.location));
                            ef.health -= gf.attackPower;
                            if(ef.health > 0){
                                tempCaveFeatures.setItem(pathToElf.location, ef);
                            } else {
                                tempCaveFeatures.setItem(pathToElf.location, empty());
                            }
                            std::cout << "elf health: " << ef.health << "\n";
                        }
                    }
                },
                [&tempCaveFeatures, &feature](elfFighter ef){
                    if(!ef.alreadyMoved){
                        ef.alreadyMoved = true;
                        pathNode pathToGoblin = findPathToNearestGoblin(feature.first, tempCaveFeatures);
                        tempCaveFeatures.setItem(grid::Point(feature.first.first, feature.first.second),empty());
                        if(pathToGoblin.firstStep == direction::UP && pathToGoblin.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first,feature.first.second-1),ef);
                        } else if(pathToGoblin.firstStep == direction::LEFT && pathToGoblin.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first-1,feature.first.second),ef);
                        } else if(pathToGoblin.firstStep == direction::RIGHT && pathToGoblin.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first+1,feature.first.second),ef);
                        } else if(pathToGoblin.firstStep == direction::DOWN && pathToGoblin.distance > 1){
                            tempCaveFeatures.setItem(grid::Point(feature.first.first,feature.first.second+1),ef);
                        } else {
                            tempCaveFeatures.setItem(feature.first,ef);
                        }

                    }
                },
                [](const wall&){},
                [](const empty&){}
            }, tempCaveSpot);
        }
        caveFeatures = tempCaveFeatures;
        for(auto& feature : caveFeatures){
            std::visit(overloaded {
                [&tempCaveFeatures, &feature](const goblinFighter& gf){
                    goblinFighter tempGoblin = gf;
                    tempGoblin.alreadyMoved = false;
                    tempCaveFeatures.setItem(feature.first,tempGoblin);
                },
                [&tempCaveFeatures, &feature](const elfFighter& ef){
                    elfFighter tempElf = ef;
                    tempElf.alreadyMoved = false;
                    tempCaveFeatures.setItem(feature.first, tempElf);
                },
                [](const wall&){},
                [](const empty&){}
            }, feature.second);
        }
        caveFeatures = tempCaveFeatures;
    }

    grid::grid<caveSpot> caveFeatures;
};

cave parseOutCave(std::vector<std::string> caveLines){
    cave output;
    int y = 0;
    for(const auto& line : caveLines){
        int x = 0;
        for(const auto& character : line){
            grid::Point curPoint(x,y);
            if(character == '#'){
                output.caveFeatures.setItem(curPoint, wall());
            } else if(character == 'G'){
                output.caveFeatures.setItem(curPoint, goblinFighter());
            } else if(character == 'E'){
                output.caveFeatures.setItem(curPoint, elfFighter());
            } else if(character == '.') {
                output.caveFeatures.setItem(curPoint, empty());
            }
            ++x;
        }
        ++y;
    }
    return output;
}


int main(){
    std::vector<std::string> caveLines = fileParse::storeEachLine("./challenges/challenge15/input.txt");
    cave myCave = parseOutCave(caveLines);
    myCave.outputCave();
    for(int i=0; i < 47; ++i){
        myCave.peformTurn();
    }
    std::cout << "\n\n";
    myCave.outputCave();

    /*myCave.peformTurn();
    std::cout << "\n\n";
    myCave.outputCave();
    myCave.peformTurn();
    std::cout << "\n\n";
    myCave.outputCave();
    myCave.peformTurn();
    std::cout << "\n\n";
    myCave.outputCave();*/


    return 0;
}
