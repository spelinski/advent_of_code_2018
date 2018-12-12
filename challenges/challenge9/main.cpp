#include "fileParser.h"
#include <iostream>
#include <regex>

std::pair<int,int> getPlayersAndEndValue(std::string singleGame){
    std::regex gameReg ("([0-9]+).*([0-9]+)");
    std::smatch myMatch;
    std::regex_search(singleGame, myMatch, gameReg);
 
    return std::pair<int,int>(std::stoi(myMatch[1]),std::stoi(myMatch[2]));
}

int main(){
    std::string singleGame = fileParse::storeEachLine("./challenges/challenge9/input.txt")[0]; 
    std::pair<int,int> playerAndEnd;
    std::cout << playerAndEnd.first << "," << playerAndEnd.second <<  "\n";
    return 0;
}
