//TODO: use something like a double linked list, shifting a giant vector in part 2 is unsustainable
#include "fileParser.h"
#include <iostream>
#include <regex>

std::pair<int,int> getPlayersAndEndValue(std::string singleGame){
    std::regex gameReg ("([0-9]+)\\s+.*\\s+([0-9]+)");
    std::smatch myMatch;
    std::regex_search(singleGame, myMatch, gameReg);
    return std::pair<int,int>(std::stoi(myMatch[1]),std::stoi(myMatch[2]));
}

struct player{
    explicit player():score(0){}
    long score;
};

class game{
    public:
        game(int finalValue):finished(false),board(1),currentMarbleIndex(0),nextMarbleValue(1),endValue(finalValue){}
        int playMarble(){
            int score = 0;
            if(finished){return 0;}
            if((nextMarbleValue % 23) != 0){
                int newIndex = (currentMarbleIndex+2) % board.size();
                if(newIndex == 0){newIndex = board.size();}
                board.insert(board.begin()+newIndex,nextMarbleValue);
                currentMarbleIndex = newIndex;
            } else {
                int newIndex = (currentMarbleIndex - 7);
                if(newIndex < 0) {
                    newIndex = (board.size()) + newIndex;
                }
                score += board[newIndex];
                board.erase(board.begin() + newIndex);
                score += nextMarbleValue;
                currentMarbleIndex = newIndex;
            }

            ++nextMarbleValue;
            if(nextMarbleValue > endValue){
                finished = true;
            }
            return score;
        }
        void printBoard(){
            for(const auto& marble : board){
                std::cout << marble << ", ";
            }
            std::cout << "\n";
        }
        bool finished;
    private:
        std::vector<int> board;
        int currentMarbleIndex;
        int nextMarbleValue;
        int endValue;
};

int main(){
    std::vector<player> players;
    std::string singleGame = fileParse::storeEachLine("./challenges/challenge9/input.txt")[0]; 
    std::pair<int,int> playerAndEnd = getPlayersAndEndValue(singleGame);
    for(int i = 0; i < playerAndEnd.first; ++i){
        players.push_back(player());
    }

    game currentGame(playerAndEnd.second);
    while(!currentGame.finished){
        for(auto& currentPlayer : players){
            currentPlayer.score += currentGame.playMarble();
        }
    }

    auto winningPlayer = std::max_element(players.begin(), players.end(), [](auto p1, auto p2){return p1.score < p2.score;});

    std::cout << "first game score: " << winningPlayer->score << "\n";

    game secondGame(playerAndEnd.second*100);
    std::vector<player> roundTwoPlayers;
    for(int i = 0; i < playerAndEnd.first; ++i){
        roundTwoPlayers.push_back(player());
    }
    while(!secondGame.finished){
        for(auto& currentPlayer : roundTwoPlayers){
            currentPlayer.score += secondGame.playMarble();
        }
    }

    auto roundTwoWinner = std::max_element(roundTwoPlayers.begin(), roundTwoPlayers.end(), [](auto p1, auto p2){return p1.score < p2.score;});

    std::cout << "second game score: " << roundTwoWinner->score << "\n"; 

    return 0;
}
