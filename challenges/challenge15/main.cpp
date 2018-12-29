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

pathNode createNode(pathNode currentPoint, grid::Point nextPoint, grid::grid<bool>& visited, direction newDirection){
    pathNode tempNode;
    tempNode.distance = currentPoint.distance + 1;
    if(currentPoint.distance > 0){
        tempNode.firstStep = currentPoint.firstStep;
    } else {
        tempNode.firstStep = newDirection;
    }
    tempNode.location = nextPoint;
    visited.setItem(tempNode.location, true);
    return tempNode;
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
            pathNode tempNode = createNode(currentPoint, nextPoint, visited, direction::UP);
            if((checkForEnemy(enemyType, encounterMap.getItem(tempNode.location)) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))) && tempNode.distance <= currentShortPath){
                currentQueue.push_back(tempNode);
            }
        }

        nextPoint = {currentPoint.location.first-1,currentPoint.location.second};
        //left
        if(isValid(nextPoint,encounterMap) && (!visited.getItem(nextPoint))){
            pathNode tempNode = createNode(currentPoint, nextPoint, visited, direction::LEFT);
            if((checkForEnemy(enemyType, encounterMap.getItem(tempNode.location)) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))) && tempNode.distance <= currentShortPath){
                currentQueue.push_back(tempNode);
            }
        }

        nextPoint = {currentPoint.location.first+1,currentPoint.location.second};
        //right
        if(isValid(nextPoint,encounterMap) && (!visited.getItem(nextPoint))){
            pathNode tempNode = createNode(currentPoint, nextPoint, visited, direction::RIGHT);
            if((checkForEnemy(enemyType, encounterMap.getItem(tempNode.location)) || std::holds_alternative<empty>(encounterMap.getItem(tempNode.location))) && tempNode.distance <= currentShortPath){
                currentQueue.push_back(tempNode);
            }
 
        }

        nextPoint = {currentPoint.location.first, currentPoint.location.second+1};
        //down
        if(isValid(nextPoint,encounterMap) && (!visited.getItem(nextPoint))){
            pathNode tempNode = createNode(currentPoint, nextPoint, visited, direction::DOWN);
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
    int distanceCompare = (minimumPath.distance > 0) ? (minimumPath.distance) : (900);
    std::list<pathNode> tempNodeList = findShortestPath(startPoint, fighterType::ELF, encounterMap, distanceCompare);
    for(const auto& tempNode : tempNodeList){
        if((tempNode.distance < minimumPath.distance || minimumPath.distance==0) && (tempNode.distance > 0)){
            minimumPath = tempNode;
        }
    }
    return minimumPath;
}

pathNode findPathToNearestGoblin(const grid::Point& startPoint, grid::grid<caveSpot>& encounterMap){
    pathNode minimumPath;
    minimumPath.distance = 0;
    int distanceCompare = (minimumPath.distance > 0) ? (minimumPath.distance) : (900);
    std::list<pathNode> tempNodeList = findShortestPath(startPoint, fighterType::GOBLIN, encounterMap, distanceCompare);
    for(const auto& tempNode : tempNodeList){
        if((tempNode.distance < minimumPath.distance || minimumPath.distance==0) && (tempNode.distance > 0)){
            minimumPath = tempNode;
        }
    }
    return minimumPath;
}

struct cave{
    cave(){}
    void outputCave(){
        int currentY = 0;
        std::list<int> health;
        for(const auto& feature : caveFeatures){
            if(feature.first.second > currentY){
                currentY = feature.first.second;
                for(const auto& heroHealth : health){
                    std::cout << " (" << heroHealth <<") ";
                }
                health.clear();
                std::cout << "\n";
            }

            std::visit(overloaded {
                [&health](const goblinFighter& gf) {
                    health.push_back(gf.health);
                    std::cout << "G";
                },
                [&health](const elfFighter& ef) {
                    health.push_back(ef.health);
                    std::cout << "E";
                },
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
                        grid::Point myNewLocation;
                        tempCaveFeatures.setItem(feature.first,empty());
                        if(pathToElf.firstStep == direction::UP && pathToElf.distance > 1){
                            myNewLocation = grid::Point(feature.first.first,feature.first.second-1);
                            tempCaveFeatures.setItem(myNewLocation, gf);
                        } else if(pathToElf.firstStep == direction::LEFT && pathToElf.distance > 1){
                            myNewLocation = grid::Point(feature.first.first-1,feature.first.second);
                            tempCaveFeatures.setItem(myNewLocation, gf);
                        } else if(pathToElf.firstStep == direction::RIGHT && pathToElf.distance > 1){
                            myNewLocation = grid::Point(feature.first.first+1,feature.first.second);
                            tempCaveFeatures.setItem(myNewLocation, gf);
                        } else if(pathToElf.firstStep == direction::DOWN && pathToElf.distance > 1){
                            myNewLocation = grid::Point(feature.first.first,feature.first.second+1);
                            tempCaveFeatures.setItem(myNewLocation, gf);
                        } else {
                            myNewLocation = feature.first;
                            tempCaveFeatures.setItem(feature.first,gf);
                        }
                        int health = 900;
                        grid::Point lowestHealthPoint;
                        caveSpot lowestHealthEnemy = empty();
                        caveSpot upSpot = tempCaveFeatures.getItem({myNewLocation.first, myNewLocation.second-1});
                        if(checkForEnemy(fighterType::ELF, upSpot)){
                            elfFighter ef = std::get<elfFighter>(upSpot);
                            if(ef.health < health){
                                health = ef.health;
                                lowestHealthEnemy = upSpot;
                                lowestHealthPoint = {myNewLocation.first, myNewLocation.second-1};
                            }
                        }
                        caveSpot leftSpot = tempCaveFeatures.getItem({myNewLocation.first-1, myNewLocation.second});
                        if(checkForEnemy(fighterType::ELF,leftSpot)){
                            elfFighter ef = std::get<elfFighter>(leftSpot);
                            if(ef.health < health){
                                health = ef.health;
                                lowestHealthEnemy = leftSpot;
                                lowestHealthPoint = {myNewLocation.first-1, myNewLocation.second};
                            }
                        }
                        caveSpot rightSpot = tempCaveFeatures.getItem({myNewLocation.first+1, myNewLocation.second});
                        if(checkForEnemy(fighterType::ELF, rightSpot)){
                            elfFighter ef = std::get<elfFighter>(rightSpot);
                            if(ef.health < health){
                                health = ef.health;
                                lowestHealthEnemy = rightSpot;
                                lowestHealthPoint = {myNewLocation.first+1, myNewLocation.second};
                            }
                        }
                        caveSpot downSpot = tempCaveFeatures.getItem({myNewLocation.first, myNewLocation.second+1});
                        if(checkForEnemy(fighterType::ELF, downSpot)){
                            elfFighter ef = std::get<elfFighter>(downSpot);
                            if(ef.health < health){
                                health = ef.health;
                                lowestHealthEnemy = downSpot;
                                lowestHealthPoint = {myNewLocation.first, myNewLocation.second+1};
                            }
                        }

                        if(std::holds_alternative<elfFighter>(lowestHealthEnemy)){
                            elfFighter ef = std::get<elfFighter>(lowestHealthEnemy);
                            ef.health -= gf.attackPower;
                            if(ef.health > 0){
                                tempCaveFeatures.setItem(lowestHealthPoint, ef);
                            } else {
                                tempCaveFeatures.setItem(lowestHealthPoint, empty());
                            }
                        }
                    }
                },
                [&tempCaveFeatures, &feature](elfFighter ef){
                    if(!ef.alreadyMoved){
                        ef.alreadyMoved = true;
                        pathNode pathToGoblin = findPathToNearestGoblin(feature.first, tempCaveFeatures);
                        tempCaveFeatures.setItem(feature.first,empty());
                        grid::Point myNewLocation;
                        if(pathToGoblin.firstStep == direction::UP && pathToGoblin.distance > 1){
                            myNewLocation = grid::Point(feature.first.first,feature.first.second-1);
                            tempCaveFeatures.setItem(myNewLocation,ef);
                        } else if(pathToGoblin.firstStep == direction::LEFT && pathToGoblin.distance > 1){
                            myNewLocation = grid::Point(feature.first.first-1,feature.first.second);
                            tempCaveFeatures.setItem(myNewLocation,ef);
                        } else if(pathToGoblin.firstStep == direction::RIGHT && pathToGoblin.distance > 1){
                            myNewLocation = grid::Point(feature.first.first+1,feature.first.second);
                            tempCaveFeatures.setItem(myNewLocation,ef);
                        } else if(pathToGoblin.firstStep == direction::DOWN && pathToGoblin.distance > 1){
                            myNewLocation = grid::Point(feature.first.first,feature.first.second+1);
                            tempCaveFeatures.setItem(myNewLocation,ef);
                        } else {
                            myNewLocation = feature.first;
                            tempCaveFeatures.setItem(feature.first,ef);
                        }
                        int health = 900;
                        grid::Point lowestHealthPoint;
                        caveSpot lowestHealthEnemy = empty();
                        caveSpot upSpot = tempCaveFeatures.getItem({myNewLocation.first, myNewLocation.second-1});
                        if(checkForEnemy(fighterType::GOBLIN,upSpot)){
                            goblinFighter gf = std::get<goblinFighter>(upSpot);
                            if(gf.health < health){
                                health = gf.health;
                                lowestHealthEnemy = upSpot;
                                lowestHealthPoint = {myNewLocation.first, myNewLocation.second-1};
                            }
                        }
                        caveSpot leftSpot = tempCaveFeatures.getItem({myNewLocation.first-1, myNewLocation.second});
                        if(checkForEnemy(fighterType::GOBLIN,leftSpot)){
                            goblinFighter gf = std::get<goblinFighter>(leftSpot);
                            if(gf.health < health){
                                health = gf.health;
                                lowestHealthEnemy = leftSpot;
                                lowestHealthPoint = {myNewLocation.first-1, myNewLocation.second};
                            }
                        }
                        caveSpot rightSpot = tempCaveFeatures.getItem({myNewLocation.first+1, myNewLocation.second});
                        if(checkForEnemy(fighterType::GOBLIN,rightSpot)){
                            goblinFighter gf = std::get<goblinFighter>(rightSpot);
                            if(gf.health < health){
                                health = gf.health;
                                lowestHealthEnemy = rightSpot;
                                lowestHealthPoint = {myNewLocation.first+1, myNewLocation.second};
                            }
                        }
                        caveSpot downSpot = tempCaveFeatures.getItem({myNewLocation.first, myNewLocation.second+1});
                        if(checkForEnemy(fighterType::GOBLIN,downSpot)){
                            goblinFighter gf = std::get<goblinFighter>(downSpot);
                            if(gf.health < health){
                                health = gf.health;
                                lowestHealthEnemy = downSpot;
                                lowestHealthPoint = {myNewLocation.first, myNewLocation.second+1};
                            }
                        }

                        if(std::holds_alternative<goblinFighter>(lowestHealthEnemy)){

                            goblinFighter gf = std::get<goblinFighter>(lowestHealthEnemy);
                            gf.health -= ef.attackPower;
                            if(gf.health > 0){
                                tempCaveFeatures.setItem(lowestHealthPoint, gf);
                            } else {
                                tempCaveFeatures.setItem(lowestHealthPoint, empty());
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
        std::cout << "turn: " << turnsCompleted << "\n";
        myCave.outputCave();
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

    return 0;
}
