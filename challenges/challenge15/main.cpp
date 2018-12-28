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
    goblinFighter():alreadyMoved(false){}
    bool alreadyMoved;
};

struct elfFighter{
    elfFighter():alreadyMoved(false){}
    bool alreadyMoved;
};

struct wall{};

struct empty{};

struct path{
    path(int maxDistance):distance(maxDistance){}
    std::list<direction> steps;
    int distance;
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using caveSpot = std::variant<goblinFighter, elfFighter, wall, empty>;

bool isValid(grid::Point testPoint, grid::grid<caveSpot> encounterMap){
    return (testPoint.first >= 0) && (testPoint.second >= 0) && (testPoint.first <= encounterMap.rbegin()->first.first) && (testPoint.second <= encounterMap.rbegin()->first.second);
}

struct pathNode{
    int distance;
    direction currentDirection;
    pathNode* previousNode;
};

void findShortestPath(grid::Point startPoint, grid::Point, const grid::grid<caveSpot>& encounterMap){
    grid::grid<bool> visited;
    int distance = 0;
    for(const auto& spot : encounterMap){
        visited.setItem(spot.first, false);
    }

    std::list<grid::Point> currentQueue;
    currentQueue.push_back(startPoint);

    while(!currentQueue.empty()){
        auto currentPoint = currentQueue.front();
        currentQueue.pop_front();
        
        //up
        if(isValid(grid::Point(currentPoint.first,currentPoint.second-1),encounterMap) && !visited.getItem(currentPoint)){
        }
    }

    std::cout << "distance: " << distance << "\n";
}

path findPathToNearestElf(const grid::Point& startPoint, const grid::grid<caveSpot>& encounterMap){
    path minimumPath(encounterMap.getSize()+1);
    for(const auto& spot : encounterMap){
        if(std::holds_alternative<elfFighter>(spot.second)){
            findShortestPath(startPoint, spot.first, encounterMap);
            /*if(tempPath.distance < minimumPath.distance){
                minimumPath = tempPath;
            }*/
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
                [&tempCaveFeatures, &feature](goblinFighter& gf){
                    if(!gf.alreadyMoved){
                        gf.alreadyMoved = true;
                        findPathToNearestElf(feature.first, tempCaveFeatures);
                        tempCaveFeatures.setItem(grid::Point(feature.first.first+1, feature.first.second),gf);
                        tempCaveFeatures.setItem(grid::Point(feature.first.first, feature.first.second),empty());
                    }
                },
                [&tempCaveFeatures, &feature](elfFighter& ef){
                    if(!ef.alreadyMoved){
                        ef.alreadyMoved = true;
                        tempCaveFeatures.setItem(grid::Point(feature.first.first+1, feature.first.second),ef);
                        tempCaveFeatures.setItem(grid::Point(feature.first.first, feature.first.second),empty());
                    }
                },
                [](const wall&){},
                [](const empty&){}
            }, tempCaveSpot);
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
    myCave.peformTurn();
    std::cout << "\n\n";
    myCave.outputCave();
    return 0;
}
