#include "grid.h"
#include "fileParser.h"
#include <iostream>
#include <variant>

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

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using caveSpot = std::variant<goblinFighter, elfFighter, wall, empty>;

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
