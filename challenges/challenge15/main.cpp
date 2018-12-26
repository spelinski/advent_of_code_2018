#include "grid.h"
#include "fileParser.h"
#include <iostream>
#include <variant>

class goblinFighter{};

class elfFighter{};

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
    return 0;
}
