#include "fileParser.h"
#include "grid.h"
#include <iostream>
#include <regex>

struct clay{};

struct underground {
    grid::grid<clay> undergroundMap;
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
            for(int y = yfirst; y <= ysecond; ++y){
                outputMap.undergroundMap.setItem({x,y},clay());
            }
        } 
    }
    return outputMap;
}

int main(){
    std::vector<std::string> clayLines = fileParse::storeEachLine("./challenges/challenge17/input.txt");
    underground waterScan = parseOutClay(clayLines);
    for(const auto& line : clayLines){
        std::cout << line << "\n";
    }
    for(const auto& spot : waterScan.undergroundMap){
        std::cout << "point: " << spot.first.first << "," << spot.first.second << "\n";
    }
    return 0;
}
