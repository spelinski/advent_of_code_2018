#include "elfAssembly.h"
#include "fileParser.h"
#include <array>
#include <iostream>
#include <regex>
#include <set>

struct sample{
    std::array<int,4> beforeRegs;
    int opCode;
    int opA;
    int opB;
    int opC;
    std::array<int,4> afterRegs;
};

struct result{
    std::array<int,4> regs;
};

bool operator==(const sample& samp, const result& res){
    return (samp.afterRegs == res.regs);
}

struct realInstruction{
    realInstruction(int op, int A, int B, int C):opCode(op),opA(A),opB(B),opC(C){}
    int opCode;
    int opA;
    int opB;
    int opC;
};

int getNumberOfSingles(std::map<int, std::set<elfAssembly::assemblyFunction> > testInput){
    int numSingle = 0;
    for(const auto& setItem : testInput){
        if(setItem.second.size() == 1){
            ++numSingle;
        }
    }
    return numSingle;
}
std::map<int, elfAssembly::assemblyFunction> figureOutOpCodeToAssembly(std::vector<sample> allSamples){
    std::map<int, std::set<elfAssembly::assemblyFunction> > currentPossibilities;
    std::map<int, std::set<elfAssembly::assemblyFunction> > alreadyDismissed;
    std::vector<elfAssembly::assemblyFunction> allElfAssemblyFunctions = elfAssembly::getAllElfAssemblyFunctions();

    for(const auto& currentSample : allSamples){
        if(currentPossibilities[currentSample.opCode].size() != 1){
            for(const auto& assemblyInstruction : allElfAssemblyFunctions){
                if(assemblyInstruction(currentSample.beforeRegs, currentSample.opA, currentSample.opB, currentSample.opC) == currentSample.afterRegs){
                    if(alreadyDismissed[currentSample.opCode].find(assemblyInstruction) == alreadyDismissed[currentSample.opCode].end()){
                        currentPossibilities[currentSample.opCode].insert(assemblyInstruction);
                    } else {
                        alreadyDismissed[currentSample.opCode].insert(assemblyInstruction);
                        currentPossibilities[currentSample.opCode].erase(assemblyInstruction);
                    }
                }
            }
        }
    }

    int numberOfSingles = 0;
    std::map<int, std::set<elfAssembly::assemblyFunction> > tempMap = currentPossibilities;
    int newNumberOfSingles = getNumberOfSingles(currentPossibilities);
    while(newNumberOfSingles > numberOfSingles){
        numberOfSingles = newNumberOfSingles;
        for(const auto& opCode : currentPossibilities){
            if(opCode.second.size() == 1){
                for(const auto& innerItem : currentPossibilities){
                    if(innerItem.first != opCode.first){
                        tempMap[innerItem.first].erase(*opCode.second.begin());
                    }
                }
            }
        }
        currentPossibilities = tempMap;
        newNumberOfSingles= getNumberOfSingles(currentPossibilities);
    }
    std::map<int, elfAssembly::assemblyFunction> mapOut;
    for(const auto& item : currentPossibilities){
        mapOut[item.first] = *item.second.begin();
    }
    return mapOut;
}

int getNumberOfDuplicates(sample currentSample){
    int numDups = 0;
    std::vector<elfAssembly::assemblyFunction> allElfAssemblyFunctions =elfAssembly::getAllElfAssemblyFunctions();
    for(const auto& assemblyInstruction : allElfAssemblyFunctions){
        if(assemblyInstruction(currentSample.beforeRegs, currentSample.opA, currentSample.opB, currentSample.opC) == currentSample.afterRegs){
            ++numDups;
        }
    }
    return numDups;
}

int getNumberOfSamplesWithGreaterOrEqualNumberOfDuplicates(std::vector<sample> allSamples, int num){
    int numSamples = 0;
    for(const auto& currentSample : allSamples){
        if(getNumberOfDuplicates(currentSample) >= num){
            ++numSamples;
        }
    }
    return numSamples;
}

std::vector<sample> parseOutSamples(std::vector<std::string> lines){
    std::vector<sample> outSamples;
    for(size_t i=0; (i+2) < lines.size(); i+=4){
        sample tempSample;
        std::regex beforeReg("Before\\:\\s+\\[([0-9]+),\\s+([0-9]+),\\s+([0-9]+),\\s+([0-9]+)");
        std::smatch myMatch;
        std::regex_search(lines[i], myMatch, beforeReg);
        tempSample.beforeRegs[0] = std::stoi(myMatch[1]);
        tempSample.beforeRegs[1] = std::stoi(myMatch[2]);
        tempSample.beforeRegs[2] = std::stoi(myMatch[3]);
        tempSample.beforeRegs[3] = std::stoi(myMatch[4]);
        std::regex opReg ("([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)");
        std::regex_search(lines[i+1], myMatch, opReg);
        tempSample.opCode = std::stoi(myMatch[1]);
        tempSample.opA = std::stoi(myMatch[2]);
        tempSample.opB = std::stoi(myMatch[3]);
        tempSample.opC = std::stoi(myMatch[4]);
        std::regex afterReg("After\\:\\s+\\[([0-9]+),\\s+([0-9]+),\\s+([0-9]+),\\s+([0-9]+)");
        std::regex_search(lines[i+2], myMatch, afterReg);
        tempSample.afterRegs[0] = std::stoi(myMatch[1]);
        tempSample.afterRegs[1] = std::stoi(myMatch[2]);
        tempSample.afterRegs[2] = std::stoi(myMatch[3]);
        tempSample.afterRegs[3] = std::stoi(myMatch[4]);
        outSamples.push_back(tempSample);
    }
    return outSamples;
}

std::vector<realInstruction> parseOutRealInstructions(std::vector<int> programLines){
    std::vector<realInstruction> outInstructions;
    for(size_t i=0; (i+3)<programLines.size(); i+=4){
        outInstructions.push_back(realInstruction(programLines[i],programLines[i+1],programLines[i+2],programLines[i+3]));
    }
    return outInstructions;
}

std::array<int, 4> runProgram(std::vector<realInstruction> program, std::map<int,elfAssembly::assemblyFunction>& opToFuncMap){
    std::array<int,4> realRegisters = {0,0,0,0};
    for(const auto& instruction : program){
        realRegisters = opToFuncMap[instruction.opCode](realRegisters,instruction.opA,instruction.opB,instruction.opC);
    }
    return realRegisters;
}

int main(){
    std::vector<std::string> lines = fileParse::storeEachLine("./challenges/challenge16/input.txt");
    std::vector<sample> allSamples = parseOutSamples(lines);
    std::cout << allSamples.size() << "\n";
    int numDups = getNumberOfSamplesWithGreaterOrEqualNumberOfDuplicates(allSamples, 3);
    std::cout << "num Dups: " << numDups << "\n";

    //part 2
    std::vector<int> programLines = fileParse::storeEachWord<int>("./challenges/challenge16/input2.txt");

    std::map<int, elfAssembly::assemblyFunction> opToFuncMap = figureOutOpCodeToAssembly(allSamples);
    std::vector<realInstruction> program = parseOutRealInstructions(programLines);
    std::array<int, 4> finalRegisters = runProgram(program, opToFuncMap);
    std::cout << "part 2: " << finalRegisters[0] << "\n";


    return 0;
}
