#include "fileParser.h"
#include <iostream>
#include <map>
#include <regex>

struct pot {
    pot():hasPlant(false){}
    pot(bool plantExist):hasPlant(plantExist){}
    bool hasPlant;
};

struct potState{
    potState(){}
    std::map<int,pot> allPots;
};

struct plantNote{
    plantNote(){}
    bool resultsInPlant;
    std::map<int, bool> genInput;
};

struct plantNotes{
    plantNotes(){}
    bool getsPlant(potState currentState, int firstIndex, int secondIndex, int thirdIndex, int fourthIndex, int fifthIndex){
        for(auto& note : allNotes){
            if(currentState.allPots[firstIndex].hasPlant != note.genInput[-2]){continue;}
            if(currentState.allPots[secondIndex].hasPlant != note.genInput[-1]){continue;}
            if(currentState.allPots[thirdIndex].hasPlant != note.genInput[0]){continue;}
            if(currentState.allPots[fourthIndex].hasPlant != note.genInput[1]){continue;}
            if(currentState.allPots[fifthIndex].hasPlant != note.genInput[2]){continue;}
            return note.resultsInPlant;
        }
        return false;
    }
    std::vector<plantNote> allNotes;
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
    std::regex noteReg ("(.*)\\s+=>\\s+([.|#])");
    for(const auto& note : plantNotesString){
        plantNote currentNote;
        std::smatch myMatch;
        std::regex_search(note, myMatch, noteReg);
        std::string input = myMatch[1];
        for(size_t i=0; i < input.size(); ++i){
            currentNote.genInput[i-2] = (input[i] == '#');
        }
        std::string result = myMatch[2];
        currentNote.resultsInPlant = (result[0] == '#');
        output.allNotes.push_back(currentNote);
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

    std::vector<std::string> initialStateAndNotes = fileParse::storeEachLine("./challenges/challenge12/input.txt");

    potState currentState = parseInitialState(initialStateAndNotes.front());
    initialStateAndNotes.erase(initialStateAndNotes.begin());
    initialStateAndNotes.erase(initialStateAndNotes.begin());
    plantNotes allMyNotes = parsePlantNotes(initialStateAndNotes);
    for(int i=0; i<20; ++i){
        printGeneration(currentState, i);
        currentState = nextGeneration(currentState,allMyNotes);
    }
    printGeneration(currentState, 20);
    int sum = sumAllPlants(currentState);
    std::cout << "sum after 20: " << sum << "\n";
    

    return 0;
}
