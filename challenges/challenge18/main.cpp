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
        void printCounts(std::ostream& outputStream){
            int open = 0;
            int tree = 0;
            int lumber = 0;
            for(const auto& optionPair : landmass){
                std::visit(overloaded {
                    [&open](const openGround&){++open;},
                    [&tree](const trees&){++tree;},
                    [&lumber](const lumberyard&){++lumber;},
                    [](const nothing&){}
                },optionPair.second);
            }
            outputStream << "open ground: " << open << ", trees: " << tree << ", lumberyards: " << lumber << "\n";
        }
        //TODO need to be careful not to invalidate iterators here
        /*void incrementMinute(){
            grid::grid<fieldOptions> copyOfLand = landmass;
            for(const auto& optionPair : copyOfLand){
            }
        }*/
        grid::grid<fieldOptions> landmass;
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
    std::vector<std::string> myInput = fileParse::storeEachLine("./challenges/challenge18/test.txt");
    fields myLumber = parseOutLumberField(myInput);
    myLumber.printFields(std::cout);
    myLumber.printCounts(std::cout);
    return 0;
}
