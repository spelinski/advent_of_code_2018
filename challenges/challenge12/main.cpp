#include "fileParser.h"
#include <iostream>
#include <map>
#include <regex>

struct pot {
    pot():hasPlant(false){}
    pot(bool plantExist):hasPlant(plantExist){}
    bool hasPlant;
};

bool operator ==(const pot& lhs, const pot& rhs){
    return (lhs.hasPlant == rhs.hasPlant);
}

struct potState{
    potState(){}
    bool operator ==(const potState& rhs){
        return allPots == rhs.allPots;
    }
    std::map<int,pot> allPots;
};

struct plantNote{
    plantNote(){}
    bool resultsInPlant;
};

struct generationKey{
    generationKey(){
        generation[0] = false;
        generation[1] = false;
        generation[2] = false;
        generation[3] = false;
        generation[4] = false;
    }
    bool generation[5];
};

bool operator<(const generationKey& lhs, const generationKey& rhs){
    for(int i = 0; i < 5; ++i){
        if(lhs.generation[i] != rhs.generation[i]) {
            if(lhs.generation[i]){
                return false;
            } else {
                return true;
            }
        }
    }
    return false;
}

struct plantNotes{
    plantNotes(){}
    bool getsPlant(potState currentState, int firstIndex, int secondIndex, int thirdIndex, int fourthIndex, int fifthIndex){
        generationKey input;
        input.generation[0] = currentState.allPots[firstIndex].hasPlant;
        input.generation[1] = currentState.allPots[secondIndex].hasPlant;
        input.generation[2] = currentState.allPots[thirdIndex].hasPlant;
        input.generation[3] = currentState.allPots[fourthIndex].hasPlant;
        input.generation[4] = currentState.allPots[fifthIndex].hasPlant;
        return allNotes[input].resultsInPlant;
    }
    std::map<generationKey,plantNote> allNotes;
};


potState parseInitialState(std::string initialStateLine){
    potState output;
    std::regex initReg (".*:\\s+(.*)");
    std::smatch myMatch;
    std::regex_search(initialStateLine, myMatch, initReg);
    std::string initialState = myMatch[1];
    for(size_t i=0; i < initialState.size(); ++i){
        if(initialState[i] == '#'){
            output.allPots[i] = pot(true);
        } else {
            output.allPots[i] = pot(false);
        }
    }
    return output;
}

plantNotes parsePlantNotes(const std::vector<std::string>& plantNotesString){
    plantNotes output;
    generationKey genKey;
    std::regex noteReg ("(.*)\\s+=>\\s+([.|#])");
    for(const auto& note : plantNotesString){
        plantNote currentNote;
        std::smatch myMatch;
        std::regex_search(note, myMatch, noteReg);
        std::string input = myMatch[1];
        for(size_t i=0; i < 5; ++i){
            genKey.generation[i] = (input[i] == '#');
        }
        std::string result = myMatch[2];
        currentNote.resultsInPlant = (result[0] == '#');
        output.allNotes[genKey] = currentNote;
    }

    return output;
}

void printGeneration(const potState& generation, int genNumber){
    std::cout << genNumber << ": ";
    for(const auto& myPot : generation.allPots){
        if(myPot.second.hasPlant){
            std::cout << "#";
        } else {
            std::cout << ".";
        }
    }
    std::cout << "\n";
}

potState nextGeneration(potState& currentGeneration, plantNotes& allNotes){
    potState newGeneration;
    if(currentGeneration.allPots.begin()->second.hasPlant){
        currentGeneration.allPots[currentGeneration.allPots.begin()->first-1] = pot(false);
        currentGeneration.allPots[currentGeneration.allPots.begin()->first-1] = pot(false);
    } else if(currentGeneration.allPots.begin()->second.hasPlant) {
        currentGeneration.allPots[currentGeneration.allPots.begin()->first-1] = pot(false);
    }
    if(currentGeneration.allPots.rbegin()->second.hasPlant){
        currentGeneration.allPots[currentGeneration.allPots.rbegin()->first+1] = pot(false);
        currentGeneration.allPots[currentGeneration.allPots.rbegin()->first+1] = pot(false);
    } else if(currentGeneration.allPots.rbegin()->second.hasPlant) {
        currentGeneration.allPots[currentGeneration.allPots.rbegin()->first+1] = pot(false);
    }

    for(const auto& myPot : currentGeneration.allPots){
        if(allNotes.getsPlant(currentGeneration,myPot.first-2,myPot.first-1,myPot.first,myPot.first+1,myPot.first+2)){
            newGeneration.allPots[myPot.first] = pot(true);
        } else {
            newGeneration.allPots[myPot.first] = pot(false);
        }
    }
    return newGeneration;
}

int sumAllPlants(const potState& currentState){
    int sum = 0;
    for(const auto& pot : currentState.allPots){
        if(pot.second.hasPlant){
            sum += pot.first;
        }
    }
    return sum;
}

int main(){
    long numberOfGenerations=200;

    std::vector<std::string> initialStateAndNotes = fileParse::storeEachLine("./challenges/challenge12/input.txt");

    potState currentState = parseInitialState(initialStateAndNotes.front());
    initialStateAndNotes.erase(initialStateAndNotes.begin());
    initialStateAndNotes.erase(initialStateAndNotes.begin());
    plantNotes allMyNotes = parsePlantNotes(initialStateAndNotes);
    for(int i=0; i<numberOfGenerations; ++i){
        //printGeneration(currentState, i);
        std::cout << "generation: " << i << ",  innerSum: " << sumAllPlants(currentState) << "\n";
        currentState = nextGeneration(currentState,allMyNotes);
    }
//    printGeneration(currentState, numberOfGenerations);
    int sum = sumAllPlants(currentState);
    std::cout << "sum: " << sum << "\n";
    

    return 0;
}
