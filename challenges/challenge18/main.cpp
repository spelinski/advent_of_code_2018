#include "fileParser.h"
#include "grid.h"
#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct nothing{};
struct openGround{};
struct trees{};
struct lumberyard{};
struct fieldCounters{
    fieldCounters():treeCount(0),openCount(0),lumberyardCount(0){};
    int treeCount;
    int openCount;
    int lumberyardCount;
};

template<class... Ts> struct overloaded : Ts... {using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
using fieldOptions = std::variant<nothing, openGround, trees, lumberyard>;

class fields {
    public:
        void printFields(std::ostream& outputStream){
            int y = 0;
            for(const auto& optionPair : landmass){
                int newY = optionPair.first.second;
                if(newY > y){
                    y = newY;
                    outputStream << "\n";
                }
                std::visit(overloaded {
                    [&outputStream](const openGround&){outputStream << ".";},
                    [&outputStream](const trees&){outputStream << "|";},
                    [&outputStream](const lumberyard&){outputStream << "#";},
                    [](const nothing&){}
                }, optionPair.second);
            }
            outputStream << "\n";
        }
        fieldCounters getCounts(){
            fieldCounters myCounters;
            for(const auto& optionPair : landmass){
                std::visit(overloaded {
                    [&myCounters](const openGround&){myCounters.openCount++;},
                    [&myCounters](const trees&){myCounters.treeCount++;},
                    [&myCounters](const lumberyard&){myCounters.lumberyardCount++;},
                    [](const nothing&){}
                },optionPair.second);
            }

            return myCounters;
        }
        void visitAdjacent(grid::Point location, grid::grid<fieldOptions>& copyOfLand){
            if(std::holds_alternative<nothing>(landmass.getItem(location))){
                return;
            } else if(std::holds_alternative<openGround>(copyOfLand.getItem(location))){
                minuteCounters.openCount++;
            }else if(std::holds_alternative<trees>(copyOfLand.getItem(location))){
                minuteCounters.treeCount++;
            }else if(std::holds_alternative<lumberyard>(copyOfLand.getItem(location))){
                minuteCounters.lumberyardCount++;
            }
        }
        void incrementMinute(){
            grid::grid<fieldOptions> copyOfLand = landmass;
            for(const auto& optionPair : copyOfLand){
                minuteCounters.openCount = 0;
                minuteCounters.treeCount = 0;
                minuteCounters.lumberyardCount = 0;
                int currentX = optionPair.first.first;
                int currentY = optionPair.first.second;
                if(std::holds_alternative<nothing>(landmass.getItem({currentX,currentY}))){
                    continue;
                }
                //check upper left
                visitAdjacent({currentX-1,currentY-1}, copyOfLand);
                //check upper middle
                visitAdjacent({currentX,currentY-1}, copyOfLand);
                //check upper right
                visitAdjacent({currentX+1,currentY-1}, copyOfLand);
                //check middle left
                visitAdjacent({currentX-1,currentY}, copyOfLand);
                //check middle right
                visitAdjacent({currentX+1,currentY}, copyOfLand);
                //check lower left
                visitAdjacent({currentX-1,currentY+1}, copyOfLand);
                //check lower middle
                visitAdjacent({currentX,currentY+1}, copyOfLand);
                //check lower right
                visitAdjacent({currentX+1,currentY+1}, copyOfLand);
                if(std::holds_alternative<openGround>(optionPair.second)){
                    if(minuteCounters.treeCount >= 3){
                        landmass.setItem(optionPair.first, trees());
                    }
                } else if(std::holds_alternative<trees>(optionPair.second)){
                    if(minuteCounters.lumberyardCount >= 3){
                        landmass.setItem(optionPair.first, lumberyard());
                    }
                } else if(std::holds_alternative<lumberyard>(optionPair.second)){
                    if(minuteCounters.lumberyardCount < 1 || minuteCounters.treeCount < 1){
                        landmass.setItem(optionPair.first, openGround());
                    }
                }
            }
        }
        grid::grid<fieldOptions> landmass;
    private:
        fieldCounters minuteCounters;
};

fields parseOutLumberField(std::vector<std::string> input){
    fields myField;
    int x = 0;
    int y = 0;
    for(const auto& row : input){
        x=0;
        for(const auto& column : row){
            if(column == '.'){
                myField.landmass.setItem({x,y}, openGround());
            } else if(column == '|') {
                myField.landmass.setItem({x,y}, trees());
            } else if(column == '#') {
                myField.landmass.setItem({x,y}, lumberyard());
            } else {
                myField.landmass.setItem({x,y}, nothing());
            }
            ++x;
        }
        ++y;
    }
    return myField;
}

int main(){
    std::vector<std::string> myInput = fileParse::storeEachLine("./challenges/challenge18/input.txt");
    fields myLumber = parseOutLumberField(myInput);
    int numberOfMinutes = 10;
    for(int i = 1; i <= numberOfMinutes; ++i){
        myLumber.incrementMinute();
    }
    myLumber.printFields(std::cout);
    myLumber.printCounts(std::cout);
    fieldCounters myCounters = myLumber.getCounts();
    int resourceValue = myCounters.treeCount * myCounters.lumberyardCount;
    std::cout << "Resource Value: " << resourceValue << "\n";
    return 0;
}
