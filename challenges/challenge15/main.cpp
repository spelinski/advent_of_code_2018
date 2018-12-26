#include "grid.h"
#include "fileParser.h"
#include <iostream>

class goblinFighter{
    public:
        goblinFighter():real(false){}
        goblinFighter(bool real):real(real){}
        bool isReal(){return real;}
    private:
        bool real;
};

class elfFighter{
    public:
        elfFighter():real(false){}
        elfFighter(bool real):real(real){}
        bool isReal(){return real;}
    private:
        bool real;
};

struct caveFeature{
    caveFeature():isWall(false){}
    caveFeature(bool isWall):isWall(isWall){}
    bool isWall;
};

struct cave{
    public:
        cave(){}
        void outputCave(){
            int currentY = 0;
            for(const auto& feature : caveFeatures){
                if(feature.first.second > currentY){
                    currentY = feature.first.second;
                    std::cout << "\n";
                }
                if(feature.second.isWall){
                    std::cout << "#"; 
                } else {
                    if(goblinArmy.getItem(feature.first).isReal()){
                        std::cout << "G";
                    } else if(elfArmy.getItem(feature.first).isReal()){
                        std::cout << "E";
                    } else {
                        std::cout << ".";
                    }
                }
            }
            std::cout << "\n";
        }
        grid::grid<caveFeature> caveFeatures;
        grid::grid<goblinFighter> goblinArmy;
        grid::grid<elfFighter> elfArmy;
};

cave parseOutCave(std::vector<std::string> caveLines){
    cave output;
    int y = 0;
    for(const auto& line : caveLines){
        int x = 0;
        for(const auto& character : line){
            grid::Point curPoint(x,y);
            if(character == '#'){
                output.caveFeatures.setItem(curPoint, caveFeature(true));
            } else if(character == 'G'){
                output.caveFeatures.setItem(curPoint, caveFeature(false));
                output.goblinArmy.setItem(curPoint, goblinFighter(true));
            } else if(character == 'E'){
                output.caveFeatures.setItem(curPoint, caveFeature(false));
                output.elfArmy.setItem(curPoint, elfFighter(true));
            } else if(character == '.') {
                output.caveFeatures.setItem(curPoint, caveFeature(false));
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
