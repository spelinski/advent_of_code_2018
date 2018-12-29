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

bool checkForEnemy(fighterType enemyType, caveSpot currentSpot){
    if(enemyType == fighterType::ELF){
        return std::holds_alternative<elfFighter>(currentSpot);
    } else {
        return std::holds_alternative<goblinFighter>(currentSpot);
    }
}

std::list<pathNode> findShortestPath(grid::Point startPoint,fighterType enemyType, grid::grid<caveSpot>& encounterMap, int currentShortPath){
    grid::grid<bool> visited;
    std::list<pathNode> returnPath;
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
        if(checkForEnemy(enemyType, encounterMap.getItem(currentPoint.location))){
            currentShortPath = currentPoint.distance;
            returnPath.push_back(currentPoint);
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
                if((checkForEnemy(enemyType, encounterMap.getItem(tempNode.location)) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))) && tempNode.distance <= currentShortPath){
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
            if((checkForEnemy(enemyType, encounterMap.getItem(tempNode.location)) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))) && tempNode.distance <= currentShortPath){
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
            if((checkForEnemy(enemyType, encounterMap.getItem(tempNode.location)) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))) && tempNode.distance <= currentShortPath){
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
            if((checkForEnemy(enemyType, encounterMap.getItem(tempNode.location)) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))) && tempNode.distance <= currentShortPath){
                currentQueue.push_back(tempNode);
            }
        } 
    }
    return returnPath;
}

pathNode findPathToNearestElf(const grid::Point& startPoint, grid::grid<caveSpot>& encounterMap){
    pathNode minimumPath;
    minimumPath.distance = 0;
    int minHealth = 900;
    int distanceCompare = (minimumPath.distance > 0) ? (minimumPath.distance) : (900);
    std::list<pathNode> tempNodeList = findShortestPath(startPoint, fighterType::ELF, encounterMap, distanceCompare);
    for(const auto& tempNode : tempNodeList){
        if((tempNode.distance < minimumPath.distance || minimumPath.distance==0) && (tempNode.distance > 0)){
            minimumPath = tempNode;
            minHealth = std::get<elfFighter>(encounterMap.getItem(tempNode.location)).health;
        } else if(((tempNode.distance == 1) || (tempNode.distance == 2)) && (std::get<elfFighter>(encounterMap.getItem(tempNode.location)).health < minHealth)){
            minimumPath = tempNode;
            minHealth = std::get<elfFighter>(encounterMap.getItem(tempNode.location)).health;
        }
    }
    return minimumPath;
}

pathNode findPathToNearestGoblin(const grid::Point& startPoint, grid::grid<caveSpot>& encounterMap){
    pathNode minimumPath;
    minimumPath.distance = 0;
    int minHealth = 900;
    int distanceCompare = (minimumPath.distance > 0) ? (minimumPath.distance) : (900);
    std::list<pathNode> tempNodeList = findShortestPath(startPoint, fighterType::GOBLIN, encounterMap, distanceCompare);
    for(const auto& tempNode : tempNodeList){
        if((tempNode.distance < minimumPath.distance || minimumPath.distance==0) && (tempNode.distance > 0)){
            minimumPath = tempNode;
            minHealth = std::get<goblinFighter>(encounterMap.getItem(tempNode.location)).health;
        } else if(((tempNode.distance == 1) || (tempNode.distance == 2)) && (std::get<goblinFighter>(encounterMap.getItem(tempNode.location)).health < minHealth)){
            minimumPath = tempNode;
            minHealth = std::get<goblinFighter>(encounterMap.getItem(tempNode.location)).health;
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

    int amountOfHealthLeftTotal(){
        int totalHealth = 0;
        for(const auto& caveSpot : caveFeatures){
            std::visit( overloaded {
                [&totalHealth](const goblinFighter& gf){
                    totalHealth += gf.health;
                },
                [&totalHealth](const elfFighter& ef){
                    totalHealth += ef.health;
                },
                [](const wall&){},
                [](const empty&){}
            }, caveSpot.second);
        }
        return totalHealth;
    }

    bool isBattleDone(){
        bool goblinsExist = false;
        bool elvesExist = false;
        for(const auto& caveSpot : caveFeatures){
            if(std::holds_alternative<goblinFighter>(caveSpot.second)){
                goblinsExist = true;
            } else if(std::holds_alternative<elfFighter>(caveSpot.second)){
                elvesExist = true;
            }
            if(goblinsExist && elvesExist){return false;}
        }
        return true;
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
                            //std::cout << "Elf loc: " << pathToElf.location.first << "," << pathToElf.location.second << " : Health: " << ef.health << "\n";
                            if(ef.health > 0){
                                tempCaveFeatures.setItem(pathToElf.location, ef);
                            } else {
                                tempCaveFeatures.setItem(pathToElf.location, empty());
                            }
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
                        if(pathToGoblin.distance == 1 || pathToGoblin.distance == 2){
                            goblinFighter gf = std::get<goblinFighter>(tempCaveFeatures.getItem(pathToGoblin.location));
                            gf.health -= ef.attackPower;
                            //std::cout << "Goblin loc: " << pathToGoblin.location.first << "," << pathToGoblin.location.second << " : Health: " << gf.health << "\n";
                            if(gf.health > 0){
                                tempCaveFeatures.setItem(pathToGoblin.location, gf);
                            } else {
                                tempCaveFeatures.setItem(pathToGoblin.location, empty());
                            }
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
    int turnsCompleted = 0;
    while(!myCave.isBattleDone()){
        myCave.peformTurn();
        //if(turnsCompleted % 20 == 0){
            std::cout << "turn: " << turnsCompleted << "\n";
            myCave.outputCave();
        //}
        ++turnsCompleted;
    }
    //Says don't count the turn combat ends
    --turnsCompleted;
    std::cout << "\n\n";
    myCave.outputCave();
    std::cout << "turns taken: " << turnsCompleted << "\n";
    int healthLeft = myCave.amountOfHealthLeftTotal();
    std::cout << "Health Left: " << healthLeft << "\n";
    int outcomeScore = healthLeft * turnsCompleted;
    std::cout << "Outcome: " << outcomeScore << "\n";

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
